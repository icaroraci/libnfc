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

#ifndef LIBNFC_ENDERECOS_H
#define LIBNFC_ENDERECOS_H

#include <libnfe/nfe.h>
#include <libnfe/sefaz.h>

/*
 * Endereços da NFC-e (modelo 65) por UF e ambiente, consultados numa
 * tabela local, sem acesso à rede (o equivalente de nfe_sefaz_endereco da
 * libnfe, que é só do modelo 55):
 *   - webservices (versão 4.00) do autorizador da UF: próprio (AM, GO, MG,
 *     MS, MT, PR, RS, SP) ou SVRS (as demais);
 *   - URL da consulta pelo QR Code (qrCode) e da consulta pela chave de
 *     acesso (urlChave), para nfc_qrcode_set_url.
 *
 *   const char *url, *qr, *chave;
 *   nfc_sefaz_endereco(NFE_UF_RJ, NFE_AMBIENTE_HOMOLOGACAO,
 *                      NFE_SERVICO_AUTORIZACAO, &url);
 *   nfc_qrcode_endereco(NFE_UF_RJ, NFE_AMBIENTE_HOMOLOGACAO, &qr, &chave);
 *   nfc_qrcode_set_url(q, qr, chave);
 *
 * A NFC-e não tem contingência em outro autorizador (SVC): a contingência
 * é offline (tpEmis 9), transmitida depois ao mesmo autorizador.
 *
 * Os textos devolvidos são estáticos (não libere nem altere). As fontes e a
 * data de captura de cada endereço ficam em docs/enderecos/ e
 * docs/ENDERECOS.md. Em erro, os ponteiros de saída ficam inalterados.
 */

/* URL do serviço (NFE_SERVICO_AUTORIZACAO a NFE_SERVICO_INUTILIZACAO) da
 * NFC-e na UF e ambiente. Retorna 0, E_ISNULL (url nulo) ou E_VALOR (UF,
 * ambiente ou serviço inválidos). */
int nfc_sefaz_endereco(nfe_uf uf, nfe_ambiente amb, nfe_servico servico,
                       const char **url);

/* URLs de consulta da NFC-e na UF e ambiente: url_qrcode (sem "?") e
 * url_chave, prontas para nfc_qrcode_set_url. Qualquer das saídas pode ser
 * NULL se não interessar, mas não as duas. Retorna 0, E_ISNULL ou E_VALOR
 * (UF ou ambiente inválidos). */
int nfc_qrcode_endereco(nfe_uf uf, nfe_ambiente amb, const char **url_qrcode,
                        const char **url_chave);

#endif /* LIBNFC_ENDERECOS_H */
