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
| Autorização síncrona | `NFeAutorizacao4` da NFC-e na SVRS, `indSinc` 1 | `cStat` 100, Autorizado o uso da NF-e, protocolo 333260002623908 |

Chave de acesso: `33261003465862000188650010000006571827492410`.

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

- Contingência offline (`tpEmis` 9) e QR Code versão 3.
- Eventos da NFC-e (cancelamento e cancelamento por substituição) e consulta.
- Produção (`tpAmb` 1).
