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

/* Testes da tabela de endereços da NFC-e (enderecos.h) */

#include <stdio.h>
#include <string.h>

#include <libnfe/erros.h>
#include <libnfc/enderecos.h>
#include <libnfc/qrcode.h>

#include "teste.h"

static const nfe_uf ufs[] = { NFE_UF_RO, NFE_UF_AC, NFE_UF_AM, NFE_UF_RR,
	                      NFE_UF_PA, NFE_UF_AP, NFE_UF_TO, NFE_UF_MA,
	                      NFE_UF_PI, NFE_UF_CE, NFE_UF_RN, NFE_UF_PB,
	                      NFE_UF_PE, NFE_UF_AL, NFE_UF_SE, NFE_UF_BA,
	                      NFE_UF_MG, NFE_UF_ES, NFE_UF_RJ, NFE_UF_SP,
	                      NFE_UF_PR, NFE_UF_SC, NFE_UF_RS, NFE_UF_MS,
	                      NFE_UF_MT, NFE_UF_GO, NFE_UF_DF };

static void verifica_ws(nfe_uf uf, nfe_ambiente amb, nfe_servico servico,
                        const char *esperado)
{
	const char *url = NULL;

	VERIFICA_INT(nfc_sefaz_endereco(uf, amb, servico, &url), 0);
	VERIFICA_STR(url, esperado);
}

/* Todas as UFs, ambientes e serviços têm endereço https, sem "?", e os
 * dois ambientes diferem */
static void testa_completa(void)
{
	nfc_qrcode *q = nfc_qrcode_new();
	size_t i;
	int amb, servico;

	for (i = 0; i < sizeof ufs / sizeof ufs[0]; i++) {
		for (servico = NFE_SERVICO_AUTORIZACAO;
		     servico <= NFE_SERVICO_INUTILIZACAO; servico++) {
			const char *prod = NULL, *hom = NULL;

			VERIFICA_INT(nfc_sefaz_endereco(
			                     ufs[i], NFE_AMBIENTE_PRODUCAO,
			                     (nfe_servico)servico, &prod),
			             0);
			VERIFICA_INT(nfc_sefaz_endereco(
			                     ufs[i], NFE_AMBIENTE_HOMOLOGACAO,
			                     (nfe_servico)servico, &hom),
			             0);
			VERIFICA(prod && strncmp(prod, "https://", 8) == 0);
			VERIFICA(hom && strncmp(hom, "https://", 8) == 0);
			VERIFICA(prod && strchr(prod, '?') == NULL);
			VERIFICA(prod && hom && strcmp(prod, hom) != 0);
		}
		/* As URLs de consulta passam pela validação do QR Code */
		for (amb = 1; amb <= 2; amb++) {
			const char *qr = NULL, *chave = NULL;

			VERIFICA_INT(nfc_qrcode_endereco(ufs[i],
			                                 (nfe_ambiente)amb, &qr,
			                                 &chave),
			             0);
			VERIFICA_INT(nfc_qrcode_set_url(q, qr, chave), 0);
		}
	}
	nfc_qrcode_free(q);
}

static void testa_valores(void)
{
	const char *qr = NULL, *chave = NULL;

	/* Confirmados na homologação real (docs/HOMOLOGACAO.md) */
	verifica_ws(NFE_UF_RJ, NFE_AMBIENTE_HOMOLOGACAO,
	            NFE_SERVICO_AUTORIZACAO,
	            "https://nfce-homologacao.svrs.rs.gov.br/ws/NfeAutorizacao/"
	            "NFeAutorizacao4.asmx");
	verifica_ws(NFE_UF_RJ, NFE_AMBIENTE_HOMOLOGACAO, NFE_SERVICO_EVENTO,
	            "https://nfce-homologacao.svrs.rs.gov.br/ws/recepcaoevento/"
	            "recepcaoevento4.asmx");
	verifica_ws(NFE_UF_RJ, NFE_AMBIENTE_PRODUCAO, NFE_SERVICO_AUTORIZACAO,
	            "https://nfce.svrs.rs.gov.br/ws/NfeAutorizacao/"
	            "NFeAutorizacao4.asmx");
	/* CE: SVRS na versão 4.00 */
	verifica_ws(
	        NFE_UF_CE, NFE_AMBIENTE_HOMOLOGACAO, NFE_SERVICO_STATUS,
	        "https://nfce-homologacao.svrs.rs.gov.br/ws/NfeStatusServico/"
	        "NfeStatusServico4.asmx");
	/* Autorizadores próprios; GO sem o "?wsdl" da página */
	verifica_ws(NFE_UF_SP, NFE_AMBIENTE_HOMOLOGACAO, NFE_SERVICO_STATUS,
	            "https://homologacao.nfce.fazenda.sp.gov.br/ws/"
	            "NFeStatusServico4.asmx");
	verifica_ws(NFE_UF_GO, NFE_AMBIENTE_PRODUCAO, NFE_SERVICO_AUTORIZACAO,
	            "https://nfe.sefaz.go.gov.br/nfe/services/NFeAutorizacao4");
	verifica_ws(NFE_UF_MT, NFE_AMBIENTE_HOMOLOGACAO, NFE_SERVICO_CONSULTA,
	            "https://homologacao.sefaz.mt.gov.br/nfcews/services/"
	            "NfeConsulta4");

	VERIFICA_INT(nfc_qrcode_endereco(NFE_UF_RJ, NFE_AMBIENTE_HOMOLOGACAO,
	                                 &qr, &chave),
	             0);
	VERIFICA_STR(
	        qr,
	        "https://consultadfe.fazenda.rj.gov.br/consultaNFCe/QRCode");
	VERIFICA_STR(chave, "www.fazenda.rj.gov.br/nfce/consulta");
	VERIFICA_INT(nfc_qrcode_endereco(NFE_UF_SP, NFE_AMBIENTE_HOMOLOGACAO,
	                                 &qr, NULL),
	             0);
	VERIFICA_STR(qr,
	             "https://www.homologacao.nfce.fazenda.sp.gov.br/qrcode");
	VERIFICA_INT(nfc_qrcode_endereco(NFE_UF_BA, NFE_AMBIENTE_PRODUCAO, NULL,
	                                 &chave),
	             0);
	VERIFICA_STR(chave, "www.sefaz.ba.gov.br/nfce/consulta");
}

static void testa_erros(void)
{
	const char *url = "preservado";

	VERIFICA_INT(nfc_sefaz_endereco(NFE_UF_RJ, NFE_AMBIENTE_HOMOLOGACAO,
	                                NFE_SERVICO_AUTORIZACAO, NULL),
	             E_ISNULL);
	VERIFICA_INT(nfc_sefaz_endereco((nfe_uf)99, NFE_AMBIENTE_HOMOLOGACAO,
	                                NFE_SERVICO_AUTORIZACAO, &url),
	             E_VALOR);
	VERIFICA_INT(nfc_sefaz_endereco(NFE_UF_RJ, (nfe_ambiente)3,
	                                NFE_SERVICO_AUTORIZACAO, &url),
	             E_VALOR);
	VERIFICA_INT(nfc_sefaz_endereco(NFE_UF_RJ, NFE_AMBIENTE_HOMOLOGACAO,
	                                (nfe_servico)99, &url),
	             E_VALOR);
	VERIFICA_STR(url, "preservado");
	VERIFICA_INT(nfc_qrcode_endereco(NFE_UF_RJ, NFE_AMBIENTE_HOMOLOGACAO,
	                                 NULL, NULL),
	             E_ISNULL);
	VERIFICA_INT(nfc_qrcode_endereco((nfe_uf)0, NFE_AMBIENTE_PRODUCAO, &url,
	                                 NULL),
	             E_VALOR);
	VERIFICA_STR(url, "preservado");
}

int main(void)
{
	testa_completa();
	testa_valores();
	testa_erros();
	TESTE_FIM();
}
