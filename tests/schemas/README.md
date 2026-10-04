# Schemas da NFC-e

A NFC-e (modelo 65) não tem pacote de schemas próprio: usa o mesmo leiaute 4.00 da NF-e, com as regras do modelo 65 (consumidor final, `tpImp` 4/5, `infNFeSupl` com o QR Code) aplicadas pela SEFAZ e pela validação da libnfe. Os arquivos daqui são cópias, **sem alteração**, dos schemas oficiais guardados no [tooldoce](https://github.com/icaroraci/tooldoce/tree/master/tests/schemas), que registra de que pacote cada um veio:

| Pasta | Conteúdo | Pacote oficial |
|---|---|---|
| `nfe/` | Leiaute da NF-e/NFC-e (`nfe_v4.00.xsd`, `leiauteNFe_v4.00.xsd`, tipos), consulta de situação, retorno da autorização e inutilização | PL_010f v1.04 (31/08/2026), com a NT 2025.002 (IBS/CBS/IS) e o CNPJ alfanumérico; consulta, retorno e inutilização do PL_010d v1.03 |
| `evento/` | Envelope genérico dos eventos (`envEvento`, `retEnvEvento`, `procEventoNFe`) | PL_010d |
| `evento_canc/` | Cancelamento (110111) | v1.01, NT 2018.004 |
| `evento_cancsubst/` | Cancelamento por substituição (110112), só da NFC-e | v1.01, NT 2018.004 |

O arquivo `tipos_v4.00.xsd` do tooldoce não foi copiado: não é oficial (é gerado para os testes da libnfe).

`SHA256SUMS` guarda o hash de cada arquivo; o CI confere (`sha256sum -c`) que nenhum foi alterado. `tests/test_nfce.c` valida as NFC-e geradas contra `nfe/`.

A configuração em [`tools/documento.json`](../../tools/documento.json) descreve estes schemas para os geradores da libnfe (tabelas do motor de grupos, diagramas, TODO), conforme o `docs/ESQUEMAS.md` do tooldoce.

## Atualizar

1. Quando o tooldoce trocar o pacote (nova nota técnica), copie os arquivos novos de `tests/schemas/` dele para as mesmas pastas daqui, sem alterar.
2. Regere os hashes: `cd tests/schemas && find . -name '*.xsd' | sort | xargs sha256sum > SHA256SUMS`.
3. Rode `make test`.
