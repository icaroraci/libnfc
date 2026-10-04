#!/bin/sh
# Compila e instala a libnfe (repositório tooldoce) num prefixo, para o CI.
#
# Contorno provisório: a libnfe ainda não instala o libnfe.pc
# (icaroraci/tooldoce#266). Se o `make install` dela não gerar o arquivo,
# este script escreve um equivalente; quando a issue for resolvida, o
# arquivo instalado pela própria libnfe passa a ser usado e este trecho
# deixa de agir.
#
# Uso: sh .github/scripts/instalar_libnfe.sh PREFIXO [REF]
#   PREFIXO  destino da instalação (ex.: "$RUNNER_TEMP/libnfe")
#   REF      branch, tag ou commit do tooldoce (padrão: master)
# Depois: export PKG_CONFIG_PATH="PREFIXO/lib/pkgconfig"

set -eu

prefixo=$1
ref=${2:-master}
fonte=$(mktemp -d)

git clone --quiet https://github.com/icaroraci/tooldoce.git "$fonte"
git -C "$fonte" checkout --quiet "$ref"
echo "libnfe: tooldoce $(git -C "$fonte" rev-parse --short HEAD) ($ref)"

make -C "$fonte" install PREFIX="$prefixo"

pc="$prefixo/lib/pkgconfig/libnfe.pc"
if [ ! -f "$pc" ]; then
	versao=$(sed -n 's/^#define NFE_VERSAO *"\(.*\)"/\1/p' \
		"$fonte/include/libnfe/versao.h")
	mkdir -p "$(dirname "$pc")"
	cat >"$pc" <<PC
prefix=$prefixo
libdir=\${prefix}/lib
includedir=\${prefix}/include

Name: libnfe
Description: Biblioteca C para emissão de NF-e (libnfe.pc provisório do CI da libnfc)
Version: $versao
Requires: libxml-2.0
Requires.private: xmlsec1-openssl libcurl
Cflags: -I\${includedir}
Libs: -L\${libdir} -lnfe
PC
	echo "libnfe: libnfe.pc provisório gravado em $pc (tooldoce#266)"
fi

rm -rf "$fonte"
