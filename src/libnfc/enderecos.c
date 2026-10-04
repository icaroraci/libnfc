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

#include <stddef.h>

#include <libnfe/erros.h>
#include <libnfc/enderecos.h>

#include "enderecos_dados.h"

/* Linha da UF na tabela, ou -1 */
static int linha_uf(nfe_uf uf)
{
	size_t i;

	for (i = 0; i < sizeof ufs / sizeof ufs[0]; i++)
		if (ufs[i].uf == uf)
			return (int)i;
	return -1;
}

/* Índice do ambiente nas tabelas (0 produção, 1 homologação), ou -1 */
static int indice_amb(nfe_ambiente amb)
{
	return amb == NFE_AMBIENTE_PRODUCAO      ? 0
	       : amb == NFE_AMBIENTE_HOMOLOGACAO ? 1
	                                         : -1;
}

int nfc_sefaz_endereco(nfe_uf uf, nfe_ambiente amb, nfe_servico servico,
                       const char **url)
{
	int i = linha_uf(uf), a = indice_amb(amb);

	if (url == NULL)
		return E_ISNULL;
	if (i < 0 || a < 0 || servico < NFE_SERVICO_AUTORIZACAO ||
	    servico > NFE_SERVICO_INUTILIZACAO)
		return E_VALOR;
	*url = webservices[a][ufs[i].autor][servico];
	return 0;
}

int nfc_qrcode_endereco(nfe_uf uf, nfe_ambiente amb, const char **url_qrcode,
                        const char **url_chave)
{
	int i = linha_uf(uf), a = indice_amb(amb);

	if (url_qrcode == NULL && url_chave == NULL)
		return E_ISNULL;
	if (i < 0 || a < 0)
		return E_VALOR;
	if (url_qrcode)
		*url_qrcode = ufs[i].qrcode[a];
	if (url_chave)
		*url_chave = ufs[i].chave[a];
	return 0;
}
