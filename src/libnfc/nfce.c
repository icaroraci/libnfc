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

#include <stdlib.h>
#include <string.h>

#include <libnfe/erros.h>
#include <libnfc/nfce.h>

#include "interno.h"

int nfc_assinar(const nfe_certificado *cert, const nfc_qrcode *q,
                const char *xml, size_t tam, char **nfce, size_t *tam_nfce)
{
	char *assinado = NULL;
	size_t tam_assinado = 0;
	int rc;

	if (cert == NULL || q == NULL || xml == NULL || nfce == NULL)
		return E_ISNULL;
	rc = nfe_assinar_xml(cert, xml, tam, &assinado, &tam_assinado);
	if (rc != 0)
		return rc;
	/* O QR Code vem depois da assinatura: na contingência offline ele
	 * usa o DigestValue (versão 2) ou é assinado com o mesmo certificado
	 * (versão 3) */
	rc = nfc_qrcode_inserir_cert(q, cert, assinado, tam_assinado, nfce,
	                             tam_nfce);
	free(assinado);
	return rc;
}

int nfc_autorizar(nfe_sefaz *s, const char *url, const char *id_lote,
                  const char *nfce, int *cstat, char *xmotivo,
                  size_t tam_xmotivo, char **proc, size_t *tam_proc)
{
	const char *notas[1];
	char *msg = NULL, *ret = NULL, *prot = NULL;
	size_t tam_ret = 0, tam_prot = 0;
	int rc;

	if (s == NULL || url == NULL || id_lote == NULL || nfce == NULL ||
	    cstat == NULL || proc == NULL)
		return E_ISNULL;
	*proc = NULL;
	notas[0] = nfce;
	rc = nfe_sefaz_msg_lote(id_lote, 1, notas, 1, &msg);
	if (rc != 0)
		return rc;
	rc = nfe_sefaz_enviar(s, url, NFE_SERVICO_AUTORIZACAO, msg, &ret,
	                      &tam_ret);
	free(msg);
	if (rc != 0)
		return rc;

	/* Lote processado: vale o cStat do protocolo da nota */
	rc = nfe_sefaz_protocolo(ret, tam_ret, NULL, &prot, &tam_prot);
	if (rc == E_VALOR) {
		/* Sem protNFe: o lote foi recusado */
		rc = nfe_sefaz_cstat(ret, tam_ret, cstat, xmotivo, tam_xmotivo);
		free(ret);
		return rc;
	}
	free(ret);
	if (rc != 0)
		return rc;
	rc = nfe_sefaz_cstat(prot, tam_prot, cstat, xmotivo, tam_xmotivo);
	if (rc == 0 && (*cstat == 100 || *cstat == 150))
		rc = nfe_sefaz_proc(nfce, strlen(nfce), prot, tam_prot, proc,
		                    tam_proc);
	free(prot);
	return rc;
}
