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

/* Funções de uso interno da biblioteca (não exportadas) */

#ifndef LIBNFC_INTERNO_H
#define LIBNFC_INTERNO_H

#include <stddef.h>

#include <libnfe/assinatura.h>
#include <libnfc/qrcode.h>

#define NFC_INTERNO __attribute__((visibility("hidden")))

/* nfc_qrcode_gerar e nfc_qrcode_inserir com o certificado da versão 3
 * offline informado à parte (nfc_assinar passa o certificado com que
 * assina a nota) */
NFC_INTERNO int nfc_qrcode_gerar_cert(const nfc_qrcode *q,
                                      const nfe_certificado *cert,
                                      const char *xml, size_t tam,
                                      char **qrcode);
NFC_INTERNO int nfc_qrcode_inserir_cert(const nfc_qrcode *q,
                                        const nfe_certificado *cert,
                                        const char *xml, size_t tam,
                                        char **saida, size_t *tam_saida);

#endif /* LIBNFC_INTERNO_H */
