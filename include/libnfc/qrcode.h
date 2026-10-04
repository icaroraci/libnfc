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

#ifndef LIBNFC_QRCODE_H
#define LIBNFC_QRCODE_H

#include <stddef.h>

#include <libnfe/assinatura.h>

/*
 * QR Code da NFC-e: grupo infNFeSupl (qrCode e urlChave), que fica entre
 * infNFe e a assinatura. A assinatura cobre só infNFe, por isso o grupo é
 * acrescentado depois de assinar, sem invalidá-la (ver nfce.h, que faz as
 * duas coisas).
 *
 * Versões do QR Code (Manual de Especificações Técnicas do DANFE NFC-e e
 * QR Code; o XSD do PL_010f aceita as duas):
 *   - 2: identificada pelo CSC (Código de Segurança do Contribuinte, que a
 *     SEFAZ da UF fornece ao emitente, com o seu identificador). Emissão
 *     normal: p=chave|2|tpAmb|cIdToken|hash; contingência offline
 *     (tpEmis 9): p=chave|2|tpAmb|dia|vNF|digVal|cIdToken|hash. O hash é o
 *     SHA-1, em hexadecimal maiúsculo, dos parâmetros seguidos do CSC.
 *   - 3 (NT 2025.001): sem CSC. Emissão normal: p=chave|3|tpAmb.
 *     Contingência offline:
 *     p=chave|3|tpAmb|dia|vNF|tp_idDest|idDest|assinatura, em que
 *     tp_idDest é 1 (CNPJ), 2 (CPF) ou 3 (idEstrangeiro) e idDest o CNPJ
 *     ou o CPF do destinatário (os dois vazios se a nota não tiver
 *     destinatário; idDest vazio para o estrangeiro), e assinatura é a
 *     assinatura RSA-SHA1, em base64, dos parâmetros anteriores (de chave
 *     até idDest, com os separadores), feita com a chave do certificado
 *     do emitente (nfc_qrcode_set_certificado; nfc_assinar usa o
 *     certificado com que assina a nota).
 *
 * Uso típico:
 *   nfc_qrcode *q = nfc_qrcode_new();
 *   nfc_qrcode_set_csc(q, "000001", "SEU-CSC");
 *   nfc_qrcode_set_url(q,
 * "https://www.homologacao.nfce.fazenda.sp.gov.br/qrcode",
 *                      "www.homologacao.nfce.fazenda.sp.gov.br/consulta");
 *   nfc_qrcode_inserir(q, assinado, tam, &com_qrcode, &tam_qrcode);
 *   nfc_qrcode_free(q);
 *
 * As URLs de cada UF e ambiente são publicadas pela SEFAZ da UF (Portal
 * da NFC-e). Os códigos de erro são os da libnfe (<libnfe/erros.h>).
 */

typedef struct nfc_qrcode nfc_qrcode;

/* Cria a configuração do QR Code (versão 2, sem CSC nem URLs). Retorna NULL
 * se faltar memória. */
nfc_qrcode *nfc_qrcode_new(void);
void nfc_qrcode_free(nfc_qrcode *q);

/* Versão do QR Code: 2 ou 3. Retorna 0, E_ISNULL ou E_VALOR. */
int nfc_qrcode_set_versao(nfc_qrcode *q, int versao);
int nfc_qrcode_get_versao(const nfc_qrcode *q);

/* CSC e o seu identificador (cIdToken, 1 a 6 dígitos; zeros à esquerda são
 * descartados, como pede o leiaute: "000001" vira "1"). O CSC tem de 1 a 36
 * caracteres ASCII visíveis, exceto '|'. A biblioteca não grava o CSC em
 * lugar algum. Retorna 0, E_ISNULL, E_TAMANHO ou E_VALOR. */
int nfc_qrcode_set_csc(nfc_qrcode *q, const char *id, const char *csc);

/* Certificado do emitente, usado só pela versão 3 em contingência offline
 * para assinar os parâmetros (a chave não sai do certificado). q guarda
 * apenas a referência: cert deve continuar existindo enquanto q for
 * usado; NULL desfaz a escolha. Retorna 0 ou E_ISNULL (q nulo). */
int nfc_qrcode_set_certificado(nfc_qrcode *q, const nfe_certificado *cert);

/* URL da consulta pelo QR Code (http:// ou https://, até 900 caracteres,
 * sem '?') e texto de urlChave (URL da consulta pela chave de acesso, 21 a
 * 85 caracteres). Retorna 0, E_ISNULL, E_TAMANHO ou E_VALOR. */
int nfc_qrcode_set_url(nfc_qrcode *q, const char *url_qrcode,
                       const char *url_chave);

/* Conteúdo do campo qrCode para a NFC-e assinada xml (tam bytes; documento
 * <NFe> modelo 65), em *qrcode (alocado e terminado em '\0'; libere com
 * free()). Os dados vêm da própria nota: chave de acesso, tpAmb, tpEmis,
 * dhEmi, vNF, a identificação do destinatário e, na contingência offline
 * da versão 2, o DigestValue da assinatura. Retorna 0, E_ISNULL, E_XML
 * (documento malformado, que não é NFC-e ou sem algum desses campos),
 * E_VALOR (falta o CSC, a URL ou, na versão 3 offline, o certificado, ou
 * a assinatura falhou) ou E_MALLOC. */
int nfc_qrcode_gerar(const nfc_qrcode *q, const char *xml, size_t tam,
                     char **qrcode);

/* Devolve em *saida (alocado e terminado em '\0'; libere com free()) a
 * NFC-e assinada xml com o grupo infNFeSupl logo após infNFe. O restante
 * do documento é copiado sem alteração, preservando a assinatura; o
 * tamanho vai em *tam_saida, se não for NULL. Retorna os códigos de
 * nfc_qrcode_gerar, e E_XML se a nota já tiver infNFeSupl. */
int nfc_qrcode_inserir(const nfc_qrcode *q, const char *xml, size_t tam,
                       char **saida, size_t *tam_saida);

#endif /* LIBNFC_QRCODE_H */
