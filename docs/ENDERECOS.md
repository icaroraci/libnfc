# Endereços da NFC-e por UF

`<libnfc/enderecos.h>` consulta uma tabela local da **NFC-e (modelo 65),
versão 4.00**, por UF e ambiente. Não faz requisições nem precisa de
certificado. A tabela cobre as 27 UFs, os seis serviços de `nfe_servico` e as
URLs de consulta (`qrCode` e `urlChave`).

```c
const char *url, *qr, *chave;

nfc_sefaz_endereco(NFE_UF_RJ, NFE_AMBIENTE_HOMOLOGACAO,
                   NFE_SERVICO_AUTORIZACAO, &url);
nfc_qrcode_endereco(NFE_UF_RJ, NFE_AMBIENTE_HOMOLOGACAO, &qr, &chave);
nfc_qrcode_set_url(q, qr, chave);
/* url vai para nfc_autorizar; não libere nem altere as strings */
```

`nfe_sefaz_endereco`, da libnfe, continua sendo a tabela do modelo 55: os
webservices da NFC-e são outros (por exemplo, `nfce-homologacao.svrs...` em
vez de `nfe-homologacao.svrs...`). A NFC-e não tem contingência em outro
autorizador (SVC); a contingência é offline (`tpEmis` 9) e a nota vai depois
ao mesmo autorizador, por isso as funções não recebem o tipo de emissão.

As funções retornam `0`, `E_ISNULL` (saída nula) ou `E_VALOR` (UF, ambiente
ou serviço inválidos). Em erro, os ponteiros de saída ficam inalterados.

## Autorizadores

| Autorizador | UFs |
|---|---|
| Próprio | AM, GO, MG, MS, MT, PR, RS, SP |
| SVRS | AC, AL, AP, BA, CE, DF, ES, MA, PA, PB, PE, PI, RJ, RN, RO, RR, SC, SE, TO |

A divisão difere da NF-e modelo 55: na NFC-e, BA, PE e MA também usam a SVRS.

## Fontes

Os dados ficam em `docs/enderecos/`, um arquivo por tabela, cada um com a
fonte, a data de captura e os ajustes feitos sobre o texto original:

| Arquivo | Conteúdo | Fonte (Portal Nacional da NFC-e, ENCAT) |
|---|---|---|
| `webservices.tsv` | URL de cada serviço, por autorizador e ambiente | [webservices-h](https://nfce.encat.org/desenvolvedor/webservices-h/) e [webservices-p](https://nfce.encat.org/desenvolvedor/webservices-p/) |
| `autorizadores.tsv` | autorizador de cada UF | as mesmas páginas, conferidas ao vivo (abaixo) |
| `consulta.tsv` | `qrCode` e `urlChave` por UF e ambiente | [URL por UF utilizada QR code](https://nfce.encat.org/desenvolvedor/qrcode/) e [URL por UF utilizada para consulta chave](https://nfce.encat.org/consulte-sua-nota-qr-code-versao-2-0/) |

O Manual de Especificações Técnicas do DANFE NFC-e e QR Code (Portal da
NF-e, versão de 24/03/2025, item 4.1) remete a essas páginas para as URLs
de consulta.

`src/libnfc/enderecos_dados.h` é gerado a partir dos `.tsv`:

    python3 tools/gerar_enderecos.py

O gerador recusa linhas malformadas, URLs fora do padrão (webservice sem
`https://` ou com `?`, `qrCode` com `?`, `urlChave` fora de 21 a 85
caracteres) e UFs, ambientes ou serviços faltando.

## Conferência

Em 4 de outubro de 2026, no WSL, com o certificado A1 de uma empresa do RJ:

- **Status do serviço em homologação**, com o `cUF` de cada UF: `cStat` 107
  nas 27 UFs, no autorizador da tabela, exceto MG. O servidor de MG usa um
  certificado da cadeia Sectigo Public Server Authentication Root R46, que o
  `ca-certificates` do Debian 12 não traz (falha de TLS do lado do cliente,
  não do endereço).
- **Controle negativo:** a SVRS respondeu `cStat` 410 (UF não atendida) para
  SP e MG, e SP respondeu 289 para o RJ. Portanto o 107 confirma que a UF é
  atendida naquele autorizador. Foi assim que se confirmou que o CE, que a
  página de homologação ainda lista com serviços 3.10 próprios, é atendido
  pela SVRS na versão 4.00.
- **Emissão no RJ** com o exemplo usando só a tabela (`url` = `auto`, sem
  `NFC_URL_QRCODE`): `cStat` 100, protocolo 333260002623946, com o QR Code
  em `https://consultadfe.fazenda.rj.gov.br/consultaNFCe/QRCode`; nota
  cancelada depois (protocolo 333260002623947). Autorização e eventos da
  SVRS também estão em `docs/HOMOLOGACAO.md`.

Os endereços de **produção** vêm das páginas oficiais e não foram
consultados.

## Atualização

As SEFAZ mudam endereços com aviso prévio; a página de QR Code do ENCAT
registra as trocas com data (GO, MG, PB, RJ e RN, por exemplo). Para
atualizar: confira a página oficial, mude o `.tsv` registrando a fonte e a
data, rode o gerador e `make test`.
