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

#ifndef LIBNFC_VERSAO_H
#define LIBNFC_VERSAO_H

/* Versão da biblioteca, no formato MAIOR.MENOR.REVISÃO[-PRÉ] (versionamento
 * semântico): a versão maior muda quando a API ou a ABI deixam de ser
 * compatíveis; enquanto ela for 0, a API ainda pode mudar a cada versão
 * menor. NFC_VERSAO_PRE marca uma pré-versão (ex.: "rc1") e fica vazio
 * numa versão final. O Makefile lê estas macros para nomear libnfc.so. */
#define NFC_VERSAO_MAIOR   1
#define NFC_VERSAO_MENOR   0
#define NFC_VERSAO_REVISAO 0
#define NFC_VERSAO_PRE     "rc1"
#define NFC_VERSAO         "1.0.0-rc1"

/* Versão da biblioteca carregada em tempo de execução (ex.: "1.0.0-rc1"),
 * que pode diferir de NFC_VERSAO, a dos headers usados na compilação. */
const char *nfc_versao(void);

#endif /* LIBNFC_VERSAO_H */
