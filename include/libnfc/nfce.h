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

#ifndef LIBNFC_NFCE_H
#define LIBNFC_NFCE_H

#include <stddef.h>

#include <libnfe/assinatura.h>
#include <libnfe/sefaz.h>
#include <libnfc/qrcode.h>

/*
 * Emissão da NFC-e (modelo 65). A nota é montada com a libnfe
 * (<libnfe/nfe_nfe.h>, com nfe_ide_set_mod(ide, NFE_MODELO_NFCE)); aqui
 * ficam os passos próprios da NFC-e:
 *
 *   nfe_nfe_xml(nota, &xml, &tam);
 *   nfc_assinar(cert, q, xml, tam, &nfce, &tam_nfce);     (assina + QR Code)
 *   nfc_autorizar(s, url, "1", nfce, &cstat, motivo, sizeof motivo,
 *                 &proc, &tam_proc);
 *   if (cstat == 100) ... guarde proc (nfeProc), imprima o DANFE ...
 *
 * A autorização da NFC-e é síncrona (um lote de uma nota, indSinc=1).
 */

/* Assina a NFC-e xml (documento <NFe> sem assinatura, tam bytes) com o
 * certificado e acrescenta o QR Code (infNFeSupl) conforme q. Devolve em
 * *nfce o documento pronto para transmitir (alocado e terminado em '\0';
 * libere com free()); o tamanho vai em *tam_nfce, se não for NULL.
 * Retorna 0 ou os códigos de nfe_assinar_xml e nfc_qrcode_inserir. */
int nfc_assinar(const nfe_certificado *cert, const nfc_qrcode *q,
                const char *xml, size_t tam, char **nfce, size_t *tam_nfce);

/* Envia a NFC-e assinada nfce (terminada em '\0') ao serviço de autorização
 * url, num lote síncrono identificado por id_lote (1 a 15 dígitos).
 * *cstat recebe o cStat do protocolo da nota (100: autorizada) ou, se a
 * SEFAZ recusar o lote sem processar a nota, o cStat do lote; xmotivo
 * (pode ser NULL) recebe o texto correspondente, como em nfe_sefaz_cstat.
 * Se a nota for autorizada (cStat 100 ou 150), *proc recebe o nfeProc
 * (nota e protocolo; libere com free()) e o tamanho vai em *tam_proc, se
 * não for NULL; senão *proc fica NULL. Retorna 0 quando houve resposta da
 * SEFAZ, autorizando ou não, ou E_ISNULL, E_VALOR, E_REDE (ver
 * nfe_sefaz_erro), E_XML ou E_MALLOC. */
int nfc_autorizar(nfe_sefaz *s, const char *url, const char *id_lote,
                  const char *nfce, int *cstat, char *xmotivo,
                  size_t tam_xmotivo, char **proc, size_t *tam_proc);

#endif /* LIBNFC_NFCE_H */
