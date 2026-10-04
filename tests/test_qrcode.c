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

/* Testes do QR Code da NFC-e (qrcode.h). Os hashes esperados foram
 * calculados à parte (Python, hashlib.sha1). */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libnfe/erros.h>
#include <libnfc/qrcode.h>

#include "teste.h"

#define URL_QR    "https://www.homologacao.nfce.fazenda.sp.gov.br/qrcode"
#define URL_CHAVE "www.homologacao.nfce.fazenda.sp.gov.br/consulta"
#define CSC       "CSCTESTE0123456789"

#define CHAVE_NORMAL  "35261012345678000195650010000000011123456780"
#define CHAVE_OFFLINE "35261012345678000195650010000000019123456780"
#define DIGEST        "ABCDEFGHIJKLMNOPQRSTUVWXYZ0="

/* NFC-e mínima, só com os campos que o QR Code usa */
static void nota(char *buf, size_t tam, const char *chave, const char *mod,
                 const char *tpemis, int assinada)
{
	snprintf(buf, tam,
	         "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
	         "<NFe xmlns=\"http://www.portalfiscal.inf.br/nfe\">"
	         "<infNFe Id=\"NFe%s\" versao=\"4.00\"><ide><cUF>35</cUF>"
	         "<mod>%s</mod><dhEmi>2026-10-03T05:00:00-03:00</dhEmi>"
	         "<tpEmis>%s</tpEmis><tpAmb>2</tpAmb></ide><total><ICMSTot>"
	         "<vNF>30.00</vNF></ICMSTot></total></infNFe>%s</NFe>",
	         chave, mod, tpemis,
	         assinada ? "<Signature xmlns=\"http://www.w3.org/2000/09/"
	                    "xmldsig#\"><SignedInfo><Reference URI=\"\">"
	                    "<DigestValue>" DIGEST "</DigestValue></Reference>"
	                    "</SignedInfo></Signature>"
	                  : "");
}

static void testa_config(void)
{
	nfc_qrcode *q = nfc_qrcode_new();

	VERIFICA(q != NULL);
	VERIFICA_INT(nfc_qrcode_get_versao(q), 2);
	VERIFICA_INT(nfc_qrcode_set_versao(q, 3), 0);
	VERIFICA_INT(nfc_qrcode_get_versao(q), 3);
	VERIFICA_INT(nfc_qrcode_set_versao(q, 1), E_VALOR);
	VERIFICA_INT(nfc_qrcode_get_versao(q), 3);
	VERIFICA_INT(nfc_qrcode_set_versao(NULL, 2), E_ISNULL);

	VERIFICA_INT(nfc_qrcode_set_csc(q, "000001", CSC), 0);
	VERIFICA_INT(nfc_qrcode_set_csc(q, "", CSC), E_TAMANHO);
	VERIFICA_INT(nfc_qrcode_set_csc(q, "1234567", CSC), E_TAMANHO);
	VERIFICA_INT(nfc_qrcode_set_csc(q, "12a", CSC), E_VALOR);
	VERIFICA_INT(nfc_qrcode_set_csc(q, "1", ""), E_TAMANHO);
	VERIFICA_INT(nfc_qrcode_set_csc(q, "1", "ABC|DEF"), E_VALOR);
	VERIFICA_INT(nfc_qrcode_set_csc(q, "1", "ABC DEF"), E_VALOR);
	VERIFICA_INT(nfc_qrcode_set_csc(
	                     q, "1", "0123456789012345678901234567890123456"),
	             E_TAMANHO);
	VERIFICA_INT(nfc_qrcode_set_csc(q, NULL, CSC), E_ISNULL);

	VERIFICA_INT(nfc_qrcode_set_url(q, URL_QR, URL_CHAVE), 0);
	VERIFICA_INT(nfc_qrcode_set_url(q, "ftp://x", URL_CHAVE), E_VALOR);
	VERIFICA_INT(nfc_qrcode_set_url(q, URL_QR "?p=", URL_CHAVE), E_VALOR);
	VERIFICA_INT(nfc_qrcode_set_url(q, URL_QR, "curta.gov.br"), E_TAMANHO);
	VERIFICA_INT(nfc_qrcode_set_url(q, URL_QR, " " URL_CHAVE), E_VALOR);
	VERIFICA_INT(nfc_qrcode_set_url(q, URL_QR, NULL), E_ISNULL);

	nfc_qrcode_free(q);
	nfc_qrcode_free(NULL);
}

static void testa_gerar(void)
{
	nfc_qrcode *q = nfc_qrcode_new();
	char xml[1024], *qr = NULL;

	/* Sem URL nem CSC */
	nota(xml, sizeof xml, CHAVE_NORMAL, "65", "1", 1);
	VERIFICA_INT(nfc_qrcode_gerar(q, xml, strlen(xml), &qr), E_VALOR);
	nfc_qrcode_set_url(q, URL_QR, URL_CHAVE);
	VERIFICA_INT(nfc_qrcode_gerar(q, xml, strlen(xml), &qr), E_VALOR);
	nfc_qrcode_set_csc(q, "000001", CSC);

	/* Versão 2, emissão normal */
	VERIFICA_INT(nfc_qrcode_gerar(q, xml, strlen(xml), &qr), 0);
	VERIFICA_STR(qr,
	             URL_QR "?p=" CHAVE_NORMAL
	                    "|2|2|1|17D647039602C5D96DA3C773BCE101DA042150CC");
	free(qr);
	qr = NULL;

	/* Versão 2, contingência offline: precisa da nota assinada */
	nota(xml, sizeof xml, CHAVE_OFFLINE, "65", "9", 1);
	VERIFICA_INT(nfc_qrcode_gerar(q, xml, strlen(xml), &qr), 0);
	VERIFICA_STR(qr, URL_QR "?p=" CHAVE_OFFLINE
	                        "|2|2|03|30.00|4142434445464748494a4b4c4d4e4f50"
	                        "5152535455565758595a303d|1|"
	                        "2CC95EF9914AF3226A833CC94B1F8C965BE7A236");
	free(qr);
	qr = NULL;
	nota(xml, sizeof xml, CHAVE_OFFLINE, "65", "9", 0);
	VERIFICA_INT(nfc_qrcode_gerar(q, xml, strlen(xml), &qr), E_XML);

	/* Versão 3: sem CSC na emissão normal; offline ainda não */
	nfc_qrcode_set_versao(q, 3);
	nota(xml, sizeof xml, CHAVE_NORMAL, "65", "1", 0);
	VERIFICA_INT(nfc_qrcode_gerar(q, xml, strlen(xml), &qr), 0);
	VERIFICA_STR(qr, URL_QR "?p=" CHAVE_NORMAL "|3|2");
	free(qr);
	qr = NULL;
	nota(xml, sizeof xml, CHAVE_OFFLINE, "65", "9", 1);
	VERIFICA_INT(nfc_qrcode_gerar(q, xml, strlen(xml), &qr), E_VALOR);

	/* Não é NFC-e, ou não é uma nota */
	nota(xml, sizeof xml, CHAVE_NORMAL, "55", "1", 1);
	VERIFICA_INT(nfc_qrcode_gerar(q, xml, strlen(xml), &qr), E_XML);
	VERIFICA_INT(nfc_qrcode_gerar(q, "<x/>", 4, &qr), E_XML);
	VERIFICA_INT(nfc_qrcode_gerar(q, "<NFe", 4, &qr), E_XML);
	VERIFICA_INT(nfc_qrcode_gerar(q, NULL, 0, &qr), E_ISNULL);
	VERIFICA(qr == NULL);

	nfc_qrcode_free(q);
}

static void testa_inserir(void)
{
	nfc_qrcode *q = nfc_qrcode_new();
	char xml[1024], *saida = NULL, *de_novo = NULL;
	const char *assin;
	size_t tam = 0;

	nfc_qrcode_set_csc(q, "1", CSC);
	nfc_qrcode_set_url(q, URL_QR, URL_CHAVE);
	nota(xml, sizeof xml, CHAVE_NORMAL, "65", "1", 1);
	VERIFICA_INT(nfc_qrcode_inserir(q, xml, strlen(xml), &saida, &tam), 0);
	VERIFICA_INT(tam, strlen(saida));
	/* infNFeSupl entre infNFe e Signature; o resto, idêntico */
	VERIFICA(strstr(saida, "</infNFe><infNFeSupl><qrCode>" URL_QR
	                       "?p=" CHAVE_NORMAL "|2|2|1|") != NULL);
	VERIFICA(strstr(saida, "</qrCode><urlChave>" URL_CHAVE
	                       "</urlChave></infNFeSupl><Signature") != NULL);
	assin = strstr(xml, "<Signature");
	VERIFICA(assin && strstr(saida, assin) != NULL);
	VERIFICA(strncmp(saida, xml,
	                 (size_t)(strstr(xml, "<Signature") - xml)) == 0);

	/* Não insere duas vezes */
	VERIFICA_INT(nfc_qrcode_inserir(q, saida, tam, &de_novo, NULL), E_XML);
	VERIFICA(de_novo == NULL);
	free(saida);

	nfc_qrcode_free(q);
}

int main(void)
{
	testa_config();
	testa_gerar();
	testa_inserir();
	TESTE_FIM();
}
