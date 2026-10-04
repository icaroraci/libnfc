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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <openssl/evp.h>

#include <libnfe/erros.h>
#include <libnfc/qrcode.h>

#define TAM_ID_CSC      6
#define TAM_CSC         36
#define TAM_URL_QRCODE  900
#define TAM_MIN_URL_CHV 21
#define TAM_URL_CHAVE   85
#define TAM_CHAVE       44

struct nfc_qrcode {
	int versao;
	char id_csc[TAM_ID_CSC + 1];
	char csc[TAM_CSC + 1];
	char url_qrcode[TAM_URL_QRCODE + 1];
	char url_chave[TAM_URL_CHAVE + 1];
};

/* Campos da nota usados no QR Code */
struct dados_nota {
	char chave[TAM_CHAVE + 1];
	char tpamb[2];
	char tpemis[2];
	char dia[3];
	char vnf[17];
	char digval[29];
};

nfc_qrcode *nfc_qrcode_new(void)
{
	nfc_qrcode *q = calloc(1, sizeof *q);

	if (q)
		q->versao = 2;
	return q;
}

void nfc_qrcode_free(nfc_qrcode *q)
{
	if (q == NULL)
		return;
	/* Não deixa o CSC na memória liberada */
	memset(q->csc, 0, sizeof q->csc);
	free(q);
}

int nfc_qrcode_set_versao(nfc_qrcode *q, int versao)
{
	if (q == NULL)
		return E_ISNULL;
	if (versao != 2 && versao != 3)
		return E_VALOR;
	q->versao = versao;
	return 0;
}

int nfc_qrcode_get_versao(const nfc_qrcode *q)
{
	return q ? q->versao : 0;
}

int nfc_qrcode_set_csc(nfc_qrcode *q, const char *id, const char *csc)
{
	size_t i, tam_id, tam_csc;

	if (q == NULL || id == NULL || csc == NULL)
		return E_ISNULL;
	tam_id = strlen(id);
	tam_csc = strlen(csc);
	if (tam_id < 1 || tam_id > TAM_ID_CSC || tam_csc < 1 ||
	    tam_csc > TAM_CSC)
		return E_TAMANHO;
	for (i = 0; i < tam_id; i++)
		if (id[i] < '0' || id[i] > '9')
			return E_VALOR;
	for (i = 0; i < tam_csc; i++)
		if (csc[i] < '!' || csc[i] > '~' || csc[i] == '|')
			return E_VALOR;

	/* cIdToken sem zeros à esquerda (o XSD aceita "0" ou [1-9][0-9]*) */
	while (id[0] == '0' && id[1] != '\0')
		id++;
	memcpy(q->id_csc, id, strlen(id) + 1);
	memcpy(q->csc, csc, tam_csc + 1);
	return 0;
}

int nfc_qrcode_set_url(nfc_qrcode *q, const char *url_qrcode,
                       const char *url_chave)
{
	size_t i, tam_qr, tam_chv;

	if (q == NULL || url_qrcode == NULL || url_chave == NULL)
		return E_ISNULL;
	tam_qr = strlen(url_qrcode);
	tam_chv = strlen(url_chave);
	if (tam_qr > TAM_URL_QRCODE || tam_chv < TAM_MIN_URL_CHV ||
	    tam_chv > TAM_URL_CHAVE)
		return E_TAMANHO;
	if (strncmp(url_qrcode, "https://", 8) != 0 &&
	    strncmp(url_qrcode, "http://", 7) != 0 &&
	    strncmp(url_qrcode, "HTTPS://", 8) != 0 &&
	    strncmp(url_qrcode, "HTTP://", 7) != 0)
		return E_VALOR;
	for (i = 0; i < tam_qr; i++)
		if (url_qrcode[i] <= ' ' || url_qrcode[i] > '~' ||
		    url_qrcode[i] == '?')
			return E_VALOR;
	/* urlChave: ASCII visível, sem espaços nas pontas */
	for (i = 0; i < tam_chv; i++)
		if (url_chave[i] < ' ' || url_chave[i] > '~')
			return E_VALOR;
	if (url_chave[0] == ' ' || url_chave[tam_chv - 1] == ' ')
		return E_VALOR;

	memcpy(q->url_qrcode, url_qrcode, tam_qr + 1);
	memcpy(q->url_chave, url_chave, tam_chv + 1);
	return 0;
}

/* Primeiro elemento filho de pai com o nome (local) informado */
static xmlNodePtr filho(xmlNodePtr pai, const char *nome)
{
	xmlNodePtr n;

	for (n = pai ? pai->children : NULL; n; n = n->next)
		if (n->type == XML_ELEMENT_NODE &&
		    xmlStrEqual(n->name, BAD_CAST nome))
			return n;
	return NULL;
}

/* Copia o texto do elemento para buf; falha se faltar ou não couber */
static int texto(xmlNodePtr n, char *buf, size_t tam)
{
	xmlChar *t;
	size_t len;

	if (n == NULL)
		return E_XML;
	t = xmlNodeGetContent(n);
	if (t == NULL)
		return E_XML;
	len = strlen((const char *)t);
	if (len == 0 || len >= tam) {
		xmlFree(t);
		return E_XML;
	}
	memcpy(buf, t, len + 1);
	xmlFree(t);
	return 0;
}

static int le_nota(const char *xml, size_t tam, struct dados_nota *d)
{
	xmlDocPtr doc;
	xmlNodePtr raiz, inf, ide, tot;
	xmlChar *id;
	char mod[3], dhemi[26];
	int rc = E_XML;

	if (tam > (size_t)0x7fffffff)
		return E_XML;
	doc = xmlReadMemory(xml, (int)tam, NULL, NULL,
	                    XML_PARSE_NONET | XML_PARSE_NOERROR |
	                            XML_PARSE_NOWARNING);
	if (doc == NULL)
		return E_XML;
	memset(d, 0, sizeof *d);
	raiz = xmlDocGetRootElement(doc);
	if (raiz == NULL || !xmlStrEqual(raiz->name, BAD_CAST "NFe"))
		goto fim;
	inf = filho(raiz, "infNFe");
	ide = filho(inf, "ide");
	tot = filho(filho(inf, "total"), "ICMSTot");
	if (inf == NULL || ide == NULL || tot == NULL)
		goto fim;

	/* Id="NFe" + chave de acesso */
	id = xmlGetProp(inf, BAD_CAST "Id");
	if (id == NULL)
		goto fim;
	if (strlen((const char *)id) != 3 + TAM_CHAVE ||
	    strncmp((const char *)id, "NFe", 3) != 0) {
		xmlFree(id);
		goto fim;
	}
	memcpy(d->chave, id + 3, TAM_CHAVE + 1);
	xmlFree(id);

	if (texto(filho(ide, "mod"), mod, sizeof mod) != 0 ||
	    strcmp(mod, "65") != 0 ||
	    texto(filho(ide, "tpAmb"), d->tpamb, sizeof d->tpamb) != 0 ||
	    texto(filho(ide, "tpEmis"), d->tpemis, sizeof d->tpemis) != 0 ||
	    texto(filho(ide, "dhEmi"), dhemi, sizeof dhemi) != 0 ||
	    strlen(dhemi) < 10 ||
	    texto(filho(tot, "vNF"), d->vnf, sizeof d->vnf) != 0)
		goto fim;
	/* Dia da emissão: AAAA-MM-DD... */
	memcpy(d->dia, dhemi + 8, 2);

	/* DigestValue, se a nota já estiver assinada */
	texto(filho(filho(filho(filho(raiz, "Signature"), "SignedInfo"),
	                  "Reference"),
	            "DigestValue"),
	      d->digval, sizeof d->digval);
	rc = 0;
fim:
	xmlFreeDoc(doc);
	return rc;
}

/* SHA-1 de a seguido de b, em hexadecimal maiúsculo (41 bytes) */
static int sha1_hex(const char *a, const char *b, char hex[41])
{
	static const char digitos[] = "0123456789ABCDEF";
	unsigned char md[EVP_MAX_MD_SIZE];
	unsigned int tam_md = 0, i;
	EVP_MD_CTX *ctx = EVP_MD_CTX_new();
	int ok;

	if (ctx == NULL)
		return E_MALLOC;
	ok = EVP_DigestInit_ex(ctx, EVP_sha1(), NULL) &&
	     EVP_DigestUpdate(ctx, a, strlen(a)) &&
	     EVP_DigestUpdate(ctx, b, strlen(b)) &&
	     EVP_DigestFinal_ex(ctx, md, &tam_md) && tam_md == 20;
	EVP_MD_CTX_free(ctx);
	if (!ok)
		return E_VALOR;
	for (i = 0; i < tam_md; i++) {
		hex[2 * i] = digitos[md[i] >> 4];
		hex[2 * i + 1] = digitos[md[i] & 0x0f];
	}
	hex[40] = '\0';
	return 0;
}

int nfc_qrcode_gerar(const nfc_qrcode *q, const char *xml, size_t tam,
                     char **qrcode)
{
	struct dados_nota d;
	/* Maior caso: offline v2, 44+2+1+2+16+56+6 mais separadores */
	char p[256], hash[41], digval_hex[57];
	int offline, rc, n;
	size_t i, tam_saida;

	if (q == NULL || xml == NULL || qrcode == NULL)
		return E_ISNULL;
	if (q->url_qrcode[0] == '\0')
		return E_VALOR;
	rc = le_nota(xml, tam, &d);
	if (rc != 0)
		return rc;
	offline = strcmp(d.tpemis, "9") == 0;

	if (q->versao == 3) {
		if (offline)
			return E_VALOR; /* assinatura RSA: ainda não suportada
			                 */
		n = snprintf(p, sizeof p, "%s|3|%s", d.chave, d.tpamb);
	} else {
		if (q->id_csc[0] == '\0' || q->csc[0] == '\0')
			return E_VALOR;
		if (offline) {
			/* digVal: DigestValue (base64) em hexadecimal */
			if (d.digval[0] == '\0')
				return E_XML;
			for (i = 0; d.digval[i]; i++)
				snprintf(digval_hex + 2 * i, 3, "%02x",
				         (unsigned char)d.digval[i]);
			n = snprintf(p, sizeof p, "%s|2|%s|%s|%s|%s|%s",
			             d.chave, d.tpamb, d.dia, d.vnf, digval_hex,
			             q->id_csc);
		} else {
			n = snprintf(p, sizeof p, "%s|2|%s|%s", d.chave,
			             d.tpamb, q->id_csc);
		}
		if (n < 0 || (size_t)n >= sizeof p)
			return E_XML;
		rc = sha1_hex(p, q->csc, hash);
		if (rc != 0)
			return rc;
		n = snprintf(p + n, sizeof p - (size_t)n, "|%s", hash);
	}
	if (n < 0 || strlen(p) >= sizeof p - 1)
		return E_XML;

	tam_saida = strlen(q->url_qrcode) + 3 + strlen(p) + 1;
	*qrcode = malloc(tam_saida);
	if (*qrcode == NULL)
		return E_MALLOC;
	snprintf(*qrcode, tam_saida, "%s?p=%s", q->url_qrcode, p);
	return 0;
}

/* Tamanho de s escapado para o conteúdo de um elemento XML */
static size_t tam_escapado(const char *s)
{
	size_t t = 0;

	for (; *s; s++)
		t += *s == '&' ? 5 : (*s == '<' || *s == '>') ? 4 : 1;
	return t;
}

static char *escreve_escapado(char *dst, const char *s)
{
	for (; *s; s++) {
		const char *e = *s == '&'   ? "&amp;"
		                : *s == '<' ? "&lt;"
		                : *s == '>' ? "&gt;"
		                            : NULL;
		if (e) {
			memcpy(dst, e, strlen(e));
			dst += strlen(e);
		} else {
			*dst++ = *s;
		}
	}
	return dst;
}

/* Última ocorrência de agulha (terminada em '\0') em xml[0..tam) */
static const char *busca_ultima(const char *xml, size_t tam, const char *agulha)
{
	size_t n = strlen(agulha), i;

	if (tam < n)
		return NULL;
	for (i = tam - n + 1; i-- > 0;)
		if (memcmp(xml + i, agulha, n) == 0)
			return xml + i;
	return NULL;
}

int nfc_qrcode_inserir(const nfc_qrcode *q, const char *xml, size_t tam,
                       char **saida, size_t *tam_saida)
{
	static const char abre[] = "<infNFeSupl><qrCode>",
	                  meio[] = "</qrCode><urlChave>",
	                  fecha[] = "</urlChave></infNFeSupl>";
	static const char fim_inf[] = "</infNFe>";
	const char *pos;
	char *qrcode = NULL, *out, *p;
	size_t antes, total;
	int rc;

	if (q == NULL || xml == NULL || saida == NULL)
		return E_ISNULL;
	if (q->url_chave[0] == '\0')
		return E_VALOR;
	if (busca_ultima(xml, tam, "<infNFeSupl") != NULL)
		return E_XML;
	rc = nfc_qrcode_gerar(q, xml, tam, &qrcode);
	if (rc != 0)
		return rc;
	pos = busca_ultima(xml, tam, fim_inf);
	if (pos == NULL) {
		free(qrcode);
		return E_XML;
	}
	antes = (size_t)(pos - xml) + strlen(fim_inf);

	total = tam + strlen(abre) + tam_escapado(qrcode) + strlen(meio) +
	        tam_escapado(q->url_chave) + strlen(fecha);
	out = malloc(total + 1);
	if (out == NULL) {
		free(qrcode);
		return E_MALLOC;
	}
	memcpy(out, xml, antes);
	p = out + antes;
	memcpy(p, abre, strlen(abre));
	p = escreve_escapado(p + strlen(abre), qrcode);
	memcpy(p, meio, strlen(meio));
	p = escreve_escapado(p + strlen(meio), q->url_chave);
	memcpy(p, fecha, strlen(fecha));
	p += strlen(fecha);
	memcpy(p, xml + antes, tam - antes);
	p += tam - antes;
	*p = '\0';
	free(qrcode);

	*saida = out;
	if (tam_saida)
		*tam_saida = (size_t)(p - out);
	return 0;
}
