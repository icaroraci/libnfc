/* Copyright (c) 2026 Gabriel Lampa da Cunha <gabriellampa@gmail.com>
 *
 * This file is part of libnfc.
 *
 * libnfc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libnfc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libnfc.  If not, see <https://www.gnu.org/licenses/>.
 * */

/* Testes da emissão da NFC-e (nfce.h): nota montada com a libnfe, assinada
 * com o certificado de teste, com QR Code, validada contra o XSD e enviada
 * ao servidor falso tests/servidor_sefaz.py (HTTPS com autenticação mútua,
 * em 127.0.0.1).
 *
 * Uso: test_nfce <diretório tests> */

#define _POSIX_C_SOURCE 200809L

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include <libnfe/erros.h>
#include <libnfe/validar.h>
#include <libnfc/nfce.h>

#include "teste.h"
#include "nota_teste.h"

#define URL_QR    "https://www.homologacao.nfce.fazenda.sp.gov.br/qrcode"
#define URL_CHAVE "www.homologacao.nfce.fazenda.sp.gov.br/consulta"

/* Inicia o servidor falso; devolve o pid e a porta em *porta (0 se não
 * foi possível) */
static pid_t inicia_servidor(const char *dir, int *porta)
{
	char script[1024], linha[32];
	int fd[2];
	pid_t pid;
	FILE *f;

	*porta = 0;
	snprintf(script, sizeof script, "%s/servidor_sefaz.py", dir);
	if (pipe(fd) != 0)
		return -1;
	pid = fork();
	if (pid < 0)
		return -1;
	if (pid == 0) {
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		execlp("python3", "python3", script, dir, (char *)NULL);
		_exit(127);
	}
	close(fd[1]);
	f = fdopen(fd[0], "r");
	if (f && fgets(linha, sizeof linha, f))
		*porta = atoi(linha);
	if (f)
		fclose(f);
	else
		close(fd[0]);
	return pid;
}

/* NFC-e assinada com QR Code; tpemis 9 para a contingência offline */
static char *nfce(const nfe_certificado *cert, const nfc_qrcode *q, int offline)
{
	nfe_nfe *n = nota(NFE_MODELO_NFCE);
	char *xml = NULL, *pronta = NULL;
	size_t tam = 0, tam_pronta = 0;

	if (offline) {
		nfe_ide *i = ide(NFE_MODELO_NFCE);
		VERIFICA_INT(nfe_ide_set_tpemis(
		                     i, NFE_EMISSAO_CONTINGENCIA_OFFLINE_NFCE),
		             0);
		VERIFICA_INT(
		        nfe_ide_set_contingencia(
		                i, T0, "SEM CONEXAO COM A INTERNET NA LOJA"),
		        0);
		VERIFICA_INT(nfe_nfe_set_ide(n, i), 0);
	}
	VERIFICA_INT(nfe_nfe_xml(n, &xml, &tam), 0);
	nfe_nfe_free(n);
	VERIFICA_INT(nfc_assinar(cert, q, xml, tam, &pronta, &tam_pronta), 0);
	VERIFICA(pronta != NULL && tam_pronta == strlen(pronta));
	free(xml);
	return pronta;
}

/* A nota pronta continua com a assinatura válida e passa no XSD e nas
 * regras da libnfe */
static void confere(nfe_validador *v, const char *xml)
{
	nfe_erros *erros = nfe_erros_new();
	int i;

	VERIFICA_INT(nfe_verificar_assinatura(xml, strlen(xml)), 0);
	VERIFICA_INT(nfe_validar_xml(v, xml, strlen(xml), erros), 0);
	for (i = 0; i < nfe_erros_qtd(erros); i++)
		fprintf(stderr, "  %s: %s\n", nfe_erros_campo(erros, i),
		        nfe_erros_msg(erros, i));
	nfe_erros_free(erros);
}

static void testa_autorizar(const nfe_certificado *cert, const char *xml,
                            const char *dir)
{
	nfe_sefaz *s = nfe_sefaz_new(cert);
	char url[128], ca[1024], motivo[128], *proc = NULL;
	size_t tam_proc = 0;
	int porta, cstat = 0;
	pid_t pid = inicia_servidor(dir, &porta);

	VERIFICA(porta > 0);
	if (porta <= 0) {
		if (pid > 0)
			kill(pid, SIGTERM);
		nfe_sefaz_free(s);
		return;
	}
	snprintf(ca, sizeof ca, "%s/certificados/servidor.pem", dir);
	VERIFICA_INT(nfe_sefaz_set_ca(s, ca), 0);

	snprintf(url, sizeof url, "https://127.0.0.1:%d/autoriza", porta);
	VERIFICA_INT(nfc_autorizar(s, url, "1", xml, &cstat, motivo,
	                           sizeof motivo, &proc, &tam_proc),
	             0);
	VERIFICA_INT(cstat, 100);
	VERIFICA_STR(motivo, "Autorizado o uso da NF-e");
	VERIFICA(proc && strstr(proc, "<nfeProc") && strstr(proc, "protNFe "));
	VERIFICA(proc && strstr(proc, "<infNFeSupl><qrCode>"));
	VERIFICA(proc && tam_proc == strlen(proc));
	free(proc);
	proc = NULL;

	/* Nota rejeitada: sem nfeProc */
	snprintf(url, sizeof url, "https://127.0.0.1:%d/rejeita", porta);
	VERIFICA_INT(nfc_autorizar(s, url, "2", xml, &cstat, motivo,
	                           sizeof motivo, &proc, NULL),
	             0);
	VERIFICA_INT(cstat, 464);
	VERIFICA(proc == NULL);

	/* Lote recusado: vale o cStat do lote */
	snprintf(url, sizeof url, "https://127.0.0.1:%d/lote", porta);
	VERIFICA_INT(
	        nfc_autorizar(s, url, "3", xml, &cstat, NULL, 0, &proc, NULL),
	        0);
	VERIFICA_INT(cstat, 225);
	VERIFICA(proc == NULL);

	/* Falha de comunicação */
	snprintf(url, sizeof url, "https://127.0.0.1:%d/fault", porta);
	VERIFICA_INT(
	        nfc_autorizar(s, url, "4", xml, &cstat, NULL, 0, &proc, NULL),
	        E_REDE);
	VERIFICA_INT(
	        nfc_autorizar(s, url, "x", xml, &cstat, NULL, 0, &proc, NULL),
	        E_VALOR);
	VERIFICA_INT(nfc_autorizar(NULL, url, "1", xml, &cstat, NULL, 0, &proc,
	                           NULL),
	             E_ISNULL);

	kill(pid, SIGTERM);
	waitpid(pid, NULL, 0);
	nfe_sefaz_free(s);
}

int main(int argc, char **argv)
{
	const char *dir = argc > 1 ? argv[1] : "tests";
	char pfx[1024];
	nfe_certificado *cert;
	nfe_validador *v = nfe_validador_new(NULL);
	nfc_qrcode *q = nfc_qrcode_new();
	char *xml, *dummy = NULL;
	int rc = 0;

	snprintf(pfx, sizeof pfx, "%s/certificados/teste.pfx", dir);
	cert = nfe_certificado_pfx(pfx, "teste", &rc);
	VERIFICA(cert != NULL);
	VERIFICA(v != NULL);
	if (cert == NULL || v == NULL || q == NULL) {
		nfe_certificado_free(cert);
		nfe_validador_free(v);
		nfc_qrcode_free(q);
		TESTE_FIM();
	}
	VERIFICA_INT(nfc_qrcode_set_csc(q, "000001", "CSCTESTE0123456789"), 0);
	VERIFICA_INT(nfc_qrcode_set_url(q, URL_QR, URL_CHAVE), 0);

	/* Emissão normal, QR Code versão 2 */
	xml = nfce(cert, q, 0);
	if (xml) {
		confere(v, xml);
		VERIFICA(strstr(xml, "|2|2|1|") != NULL);
		testa_autorizar(cert, xml, dir);
	}
	free(xml);

	/* Contingência offline (dia, vNF e digVal no QR Code) */
	xml = nfce(cert, q, 1);
	if (xml) {
		confere(v, xml);
		VERIFICA(strstr(xml, "|2|2|03|30.00|") != NULL);
	}
	free(xml);

	/* Versão 3, emissão normal */
	VERIFICA_INT(nfc_qrcode_set_versao(q, 3), 0);
	xml = nfce(cert, q, 0);
	if (xml) {
		confere(v, xml);
		VERIFICA(strstr(xml, "|3|2</qrCode>") != NULL);
	}
	free(xml);

	/* Sem configuração do QR Code */
	VERIFICA_INT(nfc_assinar(cert, NULL, "<NFe/>", 6, &dummy, NULL),
	             E_ISNULL);

	nfc_qrcode_free(q);
	nfe_validador_free(v);
	nfe_certificado_free(cert);
	TESTE_FIM();
}
