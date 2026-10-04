# Histórico de mudanças

As mudanças relevantes de cada versão ficam registradas aqui. O formato segue o [Keep a Changelog](https://keepachangelog.com/pt-BR/1.1.0/) e as versões seguem o [versionamento semântico](https://semver.org/lang/pt-BR/): a versão maior muda quando a API ou a ABI deixam de ser compatíveis, e com ela o `SONAME` da biblioteca.

## [Não lançado]

## [1.0.0-rc1] - 2026-10-04

Candidata à primeira versão estável (1.0.0). Cobre a emissão da NFC-e (modelo 65), em emissão normal e em contingência offline, testada na homologação real (ver [`docs/HOMOLOGACAO.md`](docs/HOMOLOGACAO.md)). Requer a libnfe 1.0.0-rc2 ou posterior.

### Adicionado

- Estrutura do projeto: Makefile (biblioteca `libnfc.so.1`, testes, `make install` com `libnfc.pc`), testes com AddressSanitizer e UBSan, CI com gcc e clang e `.clang-format`.
- Dependência da libnfe 1.x (1.0.0-rc2 ou posterior) pelo `pkg-config`; o QR Code versão 3 offline usa `nfe_certificado_assinar` (icaroraci/tooldoce#270).
- QR Code da NFC-e (`qrcode.h`): versão 2 com CSC (emissão normal e contingência offline) e versão 3 (emissão normal e contingência offline, com os parâmetros assinados pelo certificado do emitente; `nfc_qrcode_set_certificado`), e o grupo `infNFeSupl` inserido na nota assinada sem invalidar a assinatura.
- Emissão (`nfce.h`): `nfc_assinar` (assinatura e QR Code) e `nfc_autorizar` (lote síncrono, `cStat` e `nfeProc`).
- Endereços da NFC-e por UF e ambiente (`enderecos.h`): `nfc_sefaz_endereco` (webservices 4.00 do autorizador da UF) e `nfc_qrcode_endereco` (`qrCode` e `urlChave`), para as 27 UFs. Tabela gerada por `tools/gerar_enderecos.py` a partir de `docs/enderecos/`, com as fontes oficiais (Portal Nacional da NFC-e) e a conferência em homologação registradas em `docs/ENDERECOS.md`.
- Exemplo `examples/emitir_nfce.c`, que emite uma NFC-e de homologação; a versão do QR Code é escolhida por `NFC_QRCODE_VERSAO`, e `NFC_TPEMIS=9` emite em contingência offline. As URLs de consulta vêm da tabela pela `NFC_CUF`, e `auto` no lugar da URL usa o serviço de autorização da tabela.
- NFC-e autorizada na homologação real da SEFAZ (RJ, SVRS), em emissão normal e em contingência offline, com cancelamento e cancelamento por substituição, registrados em `docs/HOMOLOGACAO.md`.
- Versão da biblioteca em `<libnfc/versao.h>` (`NFC_VERSAO`) e em tempo de execução (`nfc_versao()`).

[Não lançado]: https://github.com/icaroraci/libnfc/compare/v1.0.0-rc1...HEAD
[1.0.0-rc1]: https://github.com/icaroraci/libnfc/releases/tag/v1.0.0-rc1
