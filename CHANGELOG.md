# Histórico de mudanças

As mudanças relevantes de cada versão ficam registradas aqui. O formato segue o [Keep a Changelog](https://keepachangelog.com/pt-BR/1.1.0/) e as versões seguem o [versionamento semântico](https://semver.org/lang/pt-BR/): a versão maior muda quando a API ou a ABI deixam de ser compatíveis, e com ela o `SONAME` da biblioteca. Enquanto a versão maior for 0, a API pode mudar a cada versão menor.

## [Não lançado]

### Adicionado

- Estrutura do projeto: Makefile (biblioteca `libnfc.so.0`, testes, `make install` com `libnfc.pc`), testes com AddressSanitizer e UBSan, CI com gcc e clang e `.clang-format`.
- Dependência da libnfe 1.x pelo `pkg-config`.
- Versão da biblioteca em `<libnfc/versao.h>` (`NFC_VERSAO`) e em tempo de execução (`nfc_versao()`).

[Não lançado]: https://github.com/icaroraci/libnfc/commits/main
