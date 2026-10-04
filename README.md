# libnfc

[![CI](https://github.com/icaroraci/libnfc/actions/workflows/ci.yml/badge.svg)](https://github.com/icaroraci/libnfc/actions/workflows/ci.yml)
[![Licença: LGPL v3+](https://img.shields.io/badge/licen%C3%A7a-LGPLv3%2B-blue.svg)](LICENSE)

Biblioteca C para emissão de NFC-e (Nota Fiscal de Consumidor Eletrônica, modelo 65).

A libnfc é construída sobre a [libnfe](https://github.com/icaroraci/tooldoce), que já monta, valida, assina e transmite o XML do leiaute 4.00 comum à NF-e e à NFC-e. Aqui fica o que é próprio da NFC-e: QR Code e CSC (`infNFeSupl`), endereços dos webservices da NFC-e por UF, contingência offline e DANFE NFC-e. O roteiro está em [`docs/ROTEIRO.md`](docs/ROTEIRO.md).

## Situação

Em desenvolvimento (0.1.0-dev). Por enquanto há só a estrutura do projeto: build, testes, CI e a ligação com a libnfe.

## Dependências

- [libnfe](https://github.com/icaroraci/tooldoce) 1.x, encontrada pelo `pkg-config` (`libnfe.pc`), com as dependências dela (libxml2, xmlsec1 com OpenSSL e libcurl).
- Compilador C99 (gcc ou clang) e GNU make.

Enquanto a libnfe não instala o `libnfe.pc` ([tooldoce#266](https://github.com/icaroraci/tooldoce/issues/266)), há duas saídas:

```sh
# 1) instalar a libnfe num prefixo com o script do CI, que grava um libnfe.pc provisório
sh .github/scripts/instalar_libnfe.sh "$HOME/.local/libnfe"
export PKG_CONFIG_PATH="$HOME/.local/libnfe/lib/pkgconfig"

# 2) ou informar as flags à mão
make LIBNFE_CFLAGS="-I/usr/local/include $(xml2-config --cflags)" LIBNFE_LIBS="-L/usr/local/lib -lnfe"
```

## Compilação

```sh
make                      # lib/libnfc.so (SONAME libnfc.so.0)
make test                 # testes com AddressSanitizer e UBSan
make install PREFIX=/usr  # biblioteca, headers em include/libnfc e libnfc.pc
```

Quem usa a biblioteca compila com `pkg-config --cflags --libs libnfc` e inclui `<libnfc/...>`.

## Contribuindo

Veja [`CONTRIBUTING.md`](CONTRIBUTING.md) e as [convenções de código](docs/CONVENCOES.md).

## Licença

LGPLv3 ou posterior ([`LICENSE`](LICENSE), que complementa a GPLv3 em [`COPYING`](COPYING)), a mesma da libnfe: a biblioteca pode ser usada em programas de qualquer licença.
