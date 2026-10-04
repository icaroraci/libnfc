# Roteiro

A libnfc depende da [libnfe](https://github.com/icaroraci/tooldoce) 1.x e não duplica nada dela. Esta página separa o que a libnfe já entrega para o modelo 65 do que falta construir aqui.

## O que a libnfe já faz pela NFC-e

| Recurso | Onde, na libnfe |
|---|---|
| Todos os grupos do leiaute 4.00 (PL_010f), com `mod` 65, `tpImp` 4/5, `tpEmis` 9 e `dest` opcional | `nfe_nfe.h`, `ide.h` (`NFE_MODELO_NFCE`, `NFE_DANFE_NFCE`, `NFE_EMISSAO_CONTINGENCIA_OFFLINE_NFCE`) |
| Grupo `infNFeSupl` (`qrCode`, `urlChave`), validado contra o padrão do XSD | `nfe_nfe_grupo(nota, "infNFeSupl")` (`grupo.h`) |
| Validação pelo XSD e regras da NFC-e (consumidor final, operação interna, DANFE NFC-e) | `validar.h` (`regra_nfce` em `validar.c`) |
| Chave de acesso, assinatura A1 | `chave.h`, `assinatura.h` |
| Envio SOAP/TLS a uma URL informada | `nfe_sefaz_enviar` (`sefaz.h`) |
| Cancelamento por substituição (110112), só NFC-e | `evento.h` |
| Exemplo de NFC-e completa | `examples/gerar_nfe.c` e `examples/assinar_nfe.c` do tooldoce |

## O que falta, na libnfc

1. ~~**QR Code e CSC**~~ (`qrcode.h`, `nfce.h`): `qrCode` nas versões 2 e 3 (normal e offline) e `urlChave`, inseridos após a assinatura; autorização síncrona com `nfeProc`. A versão 3 offline assina os parâmetros com a chave do certificado (RSA-SHA1) pela `nfe_certificado_assinar` da libnfe ([#3](https://github.com/icaroraci/libnfc/issues/3)).
2. ~~**Endereços dos webservices da NFC-e**~~ (`enderecos.h`): webservices 4.00 por UF e ambiente e as URLs de consulta do QR Code e de `urlChave`, com as fontes em `docs/ENDERECOS.md` ([#4](https://github.com/icaroraci/libnfc/issues/4)).
3. ~~**Contingência offline**~~ (`tpEmis` 9): emissão com QR Code offline (versões 2 e 3) e transmissão posterior, autorizadas na homologação real.
4. ~~**Homologação real**~~ dos serviços, em [`docs/HOMOLOGACAO.md`](HOMOLOGACAO.md), com o que ainda não foi testado listado no fim.

O DANFE NFC-e (impressão do cupom e mensagem eletrônica) fica fora do escopo da libnfc: é responsabilidade do programa emissor, que recebe o `nfeProc` autorizado.

Se algum item exigir mudança no XML comum (um setter que falta, uma regra de validação), a mudança vai para a libnfe, e a libnfc passa a exigir a versão que a trouxer.
