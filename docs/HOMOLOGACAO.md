# Testes na homologação da SEFAZ

Em **4 de outubro de 2026**, a biblioteca emitiu uma NFC-e no ambiente de
homologação real (`tpAmb` 2), para uma empresa do Rio de Janeiro (`cUF` 33),
autorizada pela SVRS. A nota foi montada com a libnfe, assinada com um
certificado A1 real (e-CNPJ), recebeu o QR Code versão 2 e foi enviada com
`nfc_autorizar`, pelo exemplo `examples/emitir_nfce.c`.

O XML não está no repositório: o certificado embutido na assinatura traz
dados pessoais do responsável pela empresa. Ficam registrados aqui só os
números que a SEFAZ devolveu.

## Resultado

| Serviço | Webservice | Resultado da SEFAZ |
|---|---|---|
| Autorização síncrona (nota A) | `NFeAutorizacao4`, `indSinc` 1 | `cStat` 100, Autorizado o uso da NF-e, protocolo 333260002623908 |
| Cancelamento (nota A) | `NFeRecepcaoEvento4`, evento 110111 | `cStat` 135, Evento registrado e vinculado a NF-e, protocolo 333260002623909 |
| Autorização síncrona (nota B, emissão normal) | `NFeAutorizacao4`, `indSinc` 1 | `cStat` 100, protocolo 333260002623910 |
| Autorização síncrona (nota D, contingência offline, `tpEmis` 9) | `NFeAutorizacao4`, `indSinc` 1 | `cStat` 100, protocolo 333260002623912 |
| Cancelamento por substituição (nota B, substituída pela D) | `NFeRecepcaoEvento4`, evento 110112 | `cStat` 135, Evento registrado e vinculado a NF-e, protocolo 333260002623913 |
| Cancelamento (nota C) | `NFeRecepcaoEvento4`, evento 110111 | `cStat` 135, protocolo 333260002623915 |
| Autorização síncrona (nota E, contingência offline) | `NFeAutorizacao4`, `indSinc` 1 | `cStat` 100, protocolo 333260002623916 |
| Cancelamento por substituição (nota E, offline, substituída por F normal ou G offline) | `NFeRecepcaoEvento4`, evento 110112 | `cStat` 920, Tipo de Emissao invalido no Cancelamento por Substituicao |

Notas usadas (série 1):

| Nota | Chave de acesso |
|---|---|
| A | 33261003465862000188650010000006571827492410 |
| B | 33261003465862000188650010000007561535695808 |
| C | 33261003465862000188650010000001411266363962 |
| D | 33261003465862000188650010000008929148958977 |
| E | 33261003465862000188650010000009329526191040 |

A nota C (emissão normal, protocolo 333260002623911) foi a primeira tentativa
de substituta: o evento 110112 com ela voltou `cStat` 920 (Tipo de Emissão
inválido no Cancelamento por Substituição), porque a substituta tem de ser
de contingência offline. A nota D foi emitida com `NFC_TPEMIS=9` e confirma
que o QR Code offline (dia, vNF e digVal) foi aceito.

**Regra do cancelamento por substituição**, confirmada pelos casos acima: a
nota cancelada tem de ser de emissão normal (`tpEmis` 1) e a substituta, de
contingência offline (`tpEmis` 9). É o caso do PDV que não recebe a resposta
da autorização, emite em contingência para a mesma venda e depois cancela a
nota normal que acabou autorizada. Uma NFC-e emitida offline não pode ser
substituída: para desfazê-la, use o cancelamento comum (110111). As notas F
e G, usadas só nessas tentativas, foram canceladas (protocolos
333260002623920 e 333260002623921).

Os eventos foram montados e assinados com a libnfe (`evento.h`) e enviados a
`https://nfce-homologacao.svrs.rs.gov.br/ws/recepcaoevento/recepcaoevento4.asmx`.
O `dhEvento` não pode ser anterior à emissão da nota (`cStat` 577).

Endereços usados:

- Autorização: `https://nfce-homologacao.svrs.rs.gov.br/ws/NfeAutorizacao/NFeAutorizacao4.asmx`,
  com a raiz ICP-Brasil v10 (ver `docs/TLS.md` do tooldoce).
- QR Code: `http://www4.fazenda.rj.gov.br/consultaNFCe/QRCode`.
- `urlChave`: `www.fazenda.rj.gov.br/nfce/consulta`.

## CSC

A homologação só aceita um CSC gerado para o ambiente **Teste** no portal da
SEFAZ-RJ (NFC-e, "Código de Segurança do Contribuinte", Gerar). Os CSCs de
produção, ou um id inexistente, dão `cStat` 462 ("Codigo identificador do CSC
no QR-Code nao cadastrado na SEFAZ"). O id vai no QR Code sem zeros à
esquerda (`000001` vira `1`).

## O que não foi testado

- QR Code versão 3.
- Consulta de protocolo e status do serviço da NFC-e.
- Produção (`tpAmb` 1).
