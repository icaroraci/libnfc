# libnfc

[![CI](https://github.com/icaroraci/libnfc/actions/workflows/ci.yml/badge.svg)](https://github.com/icaroraci/libnfc/actions/workflows/ci.yml)
[![Licença: LGPL v3+](https://img.shields.io/badge/licen%C3%A7a-LGPLv3%2B-blue.svg)](LICENSE)

Biblioteca C para emissão de NFC-e (Nota Fiscal de Consumidor Eletrônica, modelo 65).

A libnfc é construída sobre a [libnfe](https://github.com/icaroraci/tooldoce), que já monta, valida, assina e transmite o XML do leiaute 4.00 comum à NF-e e à NFC-e. Aqui fica o que é próprio da NFC-e: QR Code e CSC (`infNFeSupl`), endereços dos webservices da NFC-e por UF e contingência offline. O DANFE NFC-e fica fora do escopo: é impresso pelo programa emissor a partir do `nfeProc`. O roteiro está em [`docs/ROTEIRO.md`](docs/ROTEIRO.md).

## Situação

Em desenvolvimento (0.1.0-dev). A emissão foi autorizada na homologação real da SEFAZ (RJ, pela SVRS): ver [`docs/HOMOLOGACAO.md`](docs/HOMOLOGACAO.md).

| Recurso | Situação |
|---|---|
| QR Code versão 2 (CSC), emissão normal e contingência offline | Pronto (`qrcode.h`) |
| QR Code versão 3 (NT 2025.001, sem CSC), emissão normal e contingência offline | Pronto (`qrcode.h`); a offline é assinada com o certificado e foi autorizada na homologação real |
| Assinatura com QR Code e autorização síncrona (`nfeProc`) | Pronto (`nfce.h`), autorizado na homologação real |
| Endereços dos webservices e da consulta da NFC-e por UF | Pronto (`enderecos.h`), 27 UFs, com as fontes em [`docs/ENDERECOS.md`](docs/ENDERECOS.md) |

## Uso

A nota é montada com a libnfe, com `nfe_ide_set_mod(ide, NFE_MODELO_NFCE)`. Depois:

```c
nfc_qrcode *q = nfc_qrcode_new();
nfc_qrcode_set_csc(q, "000001", csc);              /* fornecidos pela SEFAZ da UF */
nfc_qrcode_set_url(q, url_qrcode, url_chave);      /* consulta da NFC-e na UF */

nfe_nfe_xml(nota, &xml, &tam);
nfc_assinar(cert, q, xml, tam, &nfce, NULL);       /* assina e acrescenta infNFeSupl */
nfc_autorizar(sefaz, url_autorizacao, "1", nfce,   /* lote síncrono */
              &cstat, motivo, sizeof motivo, &proc, NULL);
if (cstat == 100)
        ... guarde proc (nfeProc) ...
```

O programa completo está em [`examples/emitir_nfce.c`](examples/emitir_nfce.c).

## Dependências

- [libnfe](https://github.com/icaroraci/tooldoce) 1.x, encontrada pelo `pkg-config` (`libnfe.pc`), com as dependências dela (libxml2, xmlsec1 com OpenSSL e libcurl).
- libxml2 e OpenSSL (`libssl-dev`), usadas também diretamente.
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
make test                 # testes com AddressSanitizer e UBSan (python3 para o servidor falso da SEFAZ)
make exemplos             # examples/*.c em obj/
make install PREFIX=/usr  # biblioteca, headers em include/libnfc e libnfc.pc
```

Quem usa a biblioteca compila com `pkg-config --cflags --libs libnfc` e inclui `<libnfc/...>`.

## Contribuindo

Veja [`CONTRIBUTING.md`](CONTRIBUTING.md) e as [convenções de código](docs/CONVENCOES.md).

## Licença

LGPLv3 ou posterior ([`LICENSE`](LICENSE), que complementa a GPLv3 em [`COPYING`](COPYING)), a mesma da libnfe: a biblioteca pode ser usada em programas de qualquer licença.
