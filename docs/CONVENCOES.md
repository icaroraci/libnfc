# Convenções de código

A libnfc segue as [convenções da libnfe](https://github.com/icaroraci/tooldoce/blob/master/docs/CONVENCOES.md) (objetos opacos com `_new`/`_free`, setters validados, retorno `0` ou código de erro negativo, nenhuma impressão, textos UTF-8 com limite de tamanho), trocando o prefixo:

| Elemento | Padrão | Exemplo |
|---|---|---|
| Funções públicas | `nfc_<grupo>_<ação>[_<campo>]` | `nfc_versao` |
| Tipos opacos | `typedef struct nfc_<grupo> nfc_<grupo>;` | |
| Constantes de enum | `NFC_<NOME>_<VALOR>` | |
| Macros e constantes | `NFC_<NOME>` | `NFC_VERSAO` |
| Guardas de header | `LIBNFC_<ARQUIVO>_H` | `LIBNFC_VERSAO_H` |

- Headers públicos em `include/libnfc/`, incluídos como `<libnfc/arquivo.h>`; cada um compila sozinho (o CI confere).
- Tipos e funções da libnfe são usados diretamente (`nfe_nfe`, `nfe_grupo`, `nfe_sefaz`), sem embrulhá-los. Os códigos de erro também são os de `<libnfe/erros.h>`, até haver um erro próprio da NFC-e.
- **A biblioteca não imprime nada**; o CI falha se ela usar funções de saída da libc.

## Formatação

`.clang-format` na raiz (o mesmo da libnfe). Aplique com `make formatar`; o CI confere com `make verificar-formato`.
