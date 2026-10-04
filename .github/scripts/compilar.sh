#!/bin/sh
# Compila a biblioteca e confere os headers públicos e a ausência de
# impressão (docs/CONVENCOES.md).
#
# Uso local: sh .github/scripts/compilar.sh

set -u

falhou=0
log=$(mktemp)
libnfe_cflags=$(${PKG_CONFIG:-pkg-config} --cflags libnfe)

make || exit 1

# Cada header público deve compilar sozinho
for header in include/libnfc/*.h; do
	arquivo=$(basename "$header")

	# shellcheck disable=SC2086
	if printf '#include <libnfc/%s>\n' "$arquivo" |
		${CC:-cc} -std=c99 -Wall -Werror -fsyntax-only -Iinclude \
			$libnfe_cflags -x c - >"$log" 2>&1; then
		echo "ok: $arquivo (sozinho)"
	else
		cat "$log"
		echo "::error file=$header::$arquivo não compila sozinho"
		falhou=1
	fi
done

rm -f "$log"

# A biblioteca não pode imprimir nada: falha se ela usar funções de saída da
# libc
impressao=$(nm -D --undefined-only lib/libnfc.so |
	awk '{print $NF}' | sed 's/@.*//' |
	grep -E '^(printf|fprintf|vprintf|vfprintf|puts|fputs|putchar|perror|fwrite|__printf_chk|__fprintf_chk)$')
if [ -n "$impressao" ]; then
	echo "::error::a biblioteca usa funções de impressão: $(echo $impressao)"
	falhou=1
else
	echo "ok: a biblioteca não usa funções de impressão"
fi

exit "$falhou"
