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

/* Exemplo: emite uma NFC-e de homologação (um item pago em dinheiro,
 * Simples Nacional): monta a nota com a libnfe, assina com o certificado
 * A1, acrescenta o QR Code (versão 2, com o CSC) e, se a URL do serviço de
 * autorização for informada, envia à SEFAZ.
 *
 * Compilar e executar (na raiz do projeto):
 *   make exemplos
 *   ./obj/emitir_nfce <arquivo.pfx> <senha> <id CSC> <CSC> [url [ca.pem]]
 *
 * Sem url, escreve a NFC-e assinada na saída padrão. Com url (serviço
 * NFeAutorizacao4 da NFC-e da UF, em homologação), envia a nota e escreve
 * o nfeProc se ela for autorizada; o cStat vai para a saída de erros.
 * ca.pem (opcional) tem as autoridades certificadoras do servidor (ex.:
 * cadeia ICP-Brasil), se as do sistema não bastarem.
 *
 * Os dados do emitente vêm de variáveis de ambiente; sem elas, valem os
 * dados fictícios de São Paulo, que só servem sem url:
 *   NFC_CUF (35), NFC_UF (SP), NFC_CMUN (3550308), NFC_XMUN (SAO PAULO),
 *   NFC_CNPJ, NFC_IE, NFC_XNOME, NFC_SERIE (1), NFC_NNF (1),
 *   NFC_URL_QRCODE e NFC_URL_CHAVE (URLs de consulta da NFC-e da UF),
 *   NFC_QRCODE_VERSAO (2; na versão 3, a emissão normal não usa o CSC).
 *
 * Com o certificado de teste:
 *   ./obj/emitir_nfce tests/certificados/teste.pfx teste 1 CSCTESTE01234567
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <libnfe/erros.h>
#include <libnfe/nfe_nfe.h>
#include <libnfc/nfce.h>
#include <libnfc/versao.h>

/* Variável de ambiente ou o valor padrão */
static const char *var(const char *nome, const char *padrao)
{
	const char *v = getenv(nome);

	return v && *v ? v : padrao;
}

static nfe_nfe *monta_nota(void)
{
	nfe_nfe *nota = nfe_nfe_new();
	nfe_ide *ide = nfe_ide_new();
	nfe_emit *emit = nfe_emit_new();
	nfe_endereco *end = nfe_endereco_new();
	nfe_det *det = nfe_det_new();
	nfe_prod *prod = nfe_prod_new();
	nfe_imposto *imp = nfe_imposto_new();
	nfe_pag *pag = nfe_pag_new();
	nfe_detpag *dinheiro = nfe_detpag_new();
	int rc = 0;

	if (!nota || !ide || !emit || !end || !det || !prod || !imp || !pag ||
	    !dinheiro) {
		fprintf(stderr, "erro: %s\n", nfe_strerror(E_MALLOC));
		exit(1);
	}

	/* Identificação: NFC-e de homologação, emissão normal. cNF é
	 * aleatório para não repetir a chave entre execuções. */
	srand((unsigned)time(NULL));
	rc |= nfe_ide_set_cuf(ide, (nfe_uf)atoi(var("NFC_CUF", "35")));
	rc |= nfe_ide_set_cnf(ide, 10000000 + rand() % 89999999);
	rc |= nfe_ide_set_natop(ide, "VENDA");
	rc |= nfe_ide_set_mod(ide, NFE_MODELO_NFCE);
	rc |= nfe_ide_set_serie(ide, atoi(var("NFC_SERIE", "1")));
	rc |= nfe_ide_set_nnf(ide, atol(var("NFC_NNF", "1")));
	rc |= nfe_ide_set_dhemi(ide, time(NULL));
	rc |= nfe_ide_set_cmunfg(ide, atol(var("NFC_CMUN", "3550308")));
	rc |= nfe_ide_set_tpimp(ide, NFE_DANFE_NFCE);
	rc |= nfe_ide_set_tpamb(ide, NFE_AMBIENTE_HOMOLOGACAO);
	rc |= nfe_ide_set_indfinal(ide, NFE_CONSUMIDOR_FINAL);
	rc |= nfe_ide_set_indpres(ide, NFE_PRESENCA_PRESENCIAL);
	rc |= nfe_ide_set_verproc(ide, "libnfc " NFC_VERSAO);
	if (rc != 0)
		fprintf(stderr, "erro na identificação (ide)\n");

	/* Emitente */
	rc |= nfe_endereco_set_xlgr(end, "RUA DAS FLORES");
	rc |= nfe_endereco_set_nro(end, "123");
	rc |= nfe_endereco_set_xbairro(end, "CENTRO");
	rc |= nfe_endereco_set_cmun(end, atol(var("NFC_CMUN", "3550308")));
	rc |= nfe_endereco_set_xmun(end, var("NFC_XMUN", "SAO PAULO"));
	rc |= nfe_endereco_set_uf(end, var("NFC_UF", "SP"));
	rc |= nfe_endereco_set_cep(end, var("NFC_CEP", "01001000"));
	rc |= nfe_emit_set_cnpj(emit, var("NFC_CNPJ", "12345678000195"));
	rc |= nfe_emit_set_xnome(emit,
	                         var("NFC_XNOME", "EMPRESA DE TESTE LTDA"));
	rc |= nfe_emit_set_endereco(emit, end);
	rc |= nfe_emit_set_ie(emit, var("NFC_IE", "123456789012"));
	rc |= nfe_emit_set_crt(emit, NFE_CRT_SIMPLES_NACIONAL);
	if (rc != 0)
		fprintf(stderr, "erro no emitente (emit)\n");

	/* Item: em homologação, a descrição do primeiro item é fixa */
	rc |= nfe_prod_set_cprod(prod, "001");
	rc |= nfe_prod_set_xprod(prod, "NOTA FISCAL EMITIDA EM AMBIENTE DE "
	                               "HOMOLOGACAO - SEM VALOR FISCAL");
	rc |= nfe_prod_set_ncm(prod, "96081000");
	rc |= nfe_prod_set_cfop(prod, 5102);
	rc |= nfe_prod_set_comercial(prod, "UN", "1", "1.00", "1.00");
	rc |= nfe_prod_set_tributavel(prod, "UN", "1", "1.00");
	rc |= nfe_imposto_set_icmssn102(imp, NFE_ORIGEM_NACIONAL,
	                                NFE_CSOSN_102);
	rc |= nfe_imposto_set_pisnt(imp, NFE_CST_PC_SEM_INCIDENCIA);
	rc |= nfe_imposto_set_cofinsnt(imp, NFE_CST_PC_SEM_INCIDENCIA);
	rc |= nfe_det_set_prod(det, prod);
	rc |= nfe_det_set_imposto(det, imp);

	rc |= nfe_detpag_set_tpag(dinheiro, NFE_MEIO_DINHEIRO);
	rc |= nfe_detpag_set_vpag(dinheiro, "1.00");
	rc |= nfe_pag_add_detpag(pag, dinheiro);

	rc |= nfe_nfe_set_ide(nota, ide);
	rc |= nfe_nfe_set_emit(nota, emit);
	rc |= nfe_nfe_add_det(nota, det);
	rc |= nfe_nfe_set_transp(nota, nfe_transp_new());
	rc |= nfe_nfe_set_pag(nota, pag);
	rc |= nfe_nfe_calcular_totais(nota);
	if (rc != 0) {
		fprintf(stderr, "erro ao preencher a nota\n");
		nfe_nfe_free(nota);
		exit(1);
	}
	return nota;
}

int main(int argc, char **argv)
{
	nfe_certificado *cert;
	nfc_qrcode *q;
	nfe_nfe *nota;
	char *xml = NULL, *nfce = NULL, *proc = NULL, motivo[256];
	size_t tam = 0;
	int rc = 0, cstat = 0;

	if (argc < 5) {
		fprintf(stderr,
		        "uso: %s <arquivo.pfx> <senha> <id CSC> <CSC> "
		        "[url [ca.pem]]\n",
		        argv[0]);
		return 2;
	}
	cert = nfe_certificado_pfx(argv[1], argv[2], &rc);
	if (cert == NULL) {
		fprintf(stderr, "certificado: %s\n", nfe_strerror(rc));
		return 1;
	}

	q = nfc_qrcode_new();
	rc = q ? nfc_qrcode_set_csc(q, argv[3], argv[4]) : E_MALLOC;
	if (rc == 0)
		rc = nfc_qrcode_set_versao(q,
		                           atoi(var("NFC_QRCODE_VERSAO", "2")));
	if (rc == 0)
		rc = nfc_qrcode_set_url(
		        q,
		        var("NFC_URL_QRCODE",
		            "https://www.homologacao.nfce.fazenda.sp.gov.br/"
		            "qrcode"),
		        var("NFC_URL_CHAVE",
		            "www.homologacao.nfce.fazenda.sp.gov.br/consulta"));
	if (rc != 0) {
		fprintf(stderr, "QR Code (CSC ou URLs): %s\n",
		        nfe_strerror(rc));
		nfc_qrcode_free(q);
		nfe_certificado_free(cert);
		return 1;
	}

	nota = monta_nota();
	rc = nfe_nfe_xml(nota, &xml, &tam);
	nfe_nfe_free(nota);
	if (rc == 0)
		rc = nfc_assinar(cert, q, xml, tam, &nfce, NULL);
	free(xml);
	nfc_qrcode_free(q);
	if (rc != 0) {
		fprintf(stderr, "erro ao gerar a NFC-e: %s\n",
		        nfe_strerror(rc));
		nfe_certificado_free(cert);
		return 1;
	}

	if (argc < 6) {
		printf("%s\n", nfce);
	} else {
		nfe_sefaz *s = nfe_sefaz_new(cert);

		rc = s ? 0 : E_MALLOC;
		if (rc == 0 && argc > 6)
			rc = nfe_sefaz_set_ca(s, argv[6]);
		if (rc == 0)
			rc = nfc_autorizar(s, argv[5], "1", nfce, &cstat,
			                   motivo, sizeof motivo, &proc, NULL);
		if (rc == 0) {
			fprintf(stderr, "cStat %d: %s\n", cstat, motivo);
			printf("%s\n", proc ? proc : nfce);
		} else {
			fprintf(stderr, "erro no envio: %s %s\n",
			        nfe_strerror(rc), s ? nfe_sefaz_erro(s) : "");
		}
		free(proc);
		nfe_sefaz_free(s);
	}
	free(nfce);
	nfe_certificado_free(cert);
	return rc == 0 && (argc < 6 || cstat == 100) ? 0 : 1;
}
