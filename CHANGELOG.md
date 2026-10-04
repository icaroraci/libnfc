# Histórico de mudanças

As mudanças relevantes de cada versão ficam registradas aqui. O formato segue o [Keep a Changelog](https://keepachangelog.com/pt-BR/1.1.0/) e as versões seguem o [versionamento semântico](https://semver.org/lang/pt-BR/): a versão maior muda quando a API ou a ABI deixam de ser compatíveis, e com ela o `SONAME` da biblioteca. Enquanto a versão maior for 0, a API pode mudar a cada versão menor.

## [Não lançado]

### Adicionado

- Estrutura do projeto: Makefile (biblioteca `libnfc.so.0`, testes, `make install` com `libnfc.pc`), testes com AddressSanitizer e UBSan, CI com gcc e clang e `.clang-format`.
- Dependência da libnfe 1.x pelo `pkg-config`.
- QR Code da NFC-e (`qrcode.h`): versão 2 com CSC (emissão normal e contingência offline) e versão 3 (emissão normal), e o grupo `infNFeSupl` inserido na nota assinada sem invalidar a assinatura.
- Emissão (`nfce.h`): `nfc_assinar` (assinatura e QR Code) e `nfc_autorizar` (lote síncrono, `cStat` e `nfeProc`).
- Exemplo `examples/emitir_nfce.c`, que emite uma NFC-e de homologação; a versão do QR Code é escolhida por `NFC_QRCODE_VERSAO`.
- NFC-e autorizada na homologação real da SEFAZ (RJ, SVRS), registrada em `docs/HOMOLOGACAO.md`.
- Versão da biblioteca em `<libnfc/versao.h>` (`NFC_VERSAO`) e em tempo de execução (`nfc_versao()`).

[Não lançado]: https://github.com/icaroraci/libnfc/commits/main
