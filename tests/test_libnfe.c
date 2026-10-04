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

/* Testes da dependência libnfe: os headers são encontrados, a biblioteca
 * é ligada e aceita o modelo 65 */

#include <libnfe/ide.h>
#include <libnfe/versao.h>

#include "teste.h"

int main(void)
{
	nfe_ide *ide = nfe_ide_new();

	/* libnfe 1.x (SONAME libnfe.so.1) */
	VERIFICA_INT(NFE_VERSAO_MAIOR, 1);
	VERIFICA(nfe_versao() != NULL && nfe_versao()[0] == '1');

	VERIFICA(ide != NULL);
	VERIFICA_INT(nfe_ide_set_mod(ide, NFE_MODELO_NFCE), 0);
	nfe_ide_free(ide);
	TESTE_FIM();
}
