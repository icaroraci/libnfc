# Como contribuir

Ao contribuir com este repositório, discuta antes a mudança que você deseja fazer pelas [issues](https://github.com/icaroraci/libnfc/issues), por <a href="mailto:gabriellampa@gmail.com">e-mail</a> ou pelo Telegram (@GabrielLampa).

Siga o [código de conduta](CODE_OF_CONDUCT.md) em todas as interações com o projeto.

Mudanças no XML comum à NF-e e à NFC-e (grupos do leiaute, validação, assinatura, comunicação SOAP) pertencem à [libnfe](https://github.com/icaroraci/tooldoce); aqui fica só o que é próprio da NFC-e (ver [`docs/ROTEIRO.md`](docs/ROTEIRO.md)).

## Enviando alterações

Envie PRs com mudanças significativas com uma lista clara do que você fez. Mensagens de commit de uma linha servem para mudanças pequenas; as maiores levam um resumo e um parágrafo descrevendo o que mudou e o impacto:

    $ git commit -m "Um breve resumo do commit
    >
    > Um parágrafo descrevendo o que mudou e seu impacto."

Antes de enviar, rode:

    $ make test
    $ make verificar-formato   # ou make formatar para corrigir

Todo arquivo `.c` e `.h` novo começa com o cabeçalho de licença (LGPLv3+) usado nos demais.

## Convenções de código

Veja [`docs/CONVENCOES.md`](docs/CONVENCOES.md).

## Fluxo com fork

As alterações chegam ao projeto por pull request a partir de um fork na sua conta do GitHub.

1. Use o botão *Fork* na página do [repositório](https://github.com/icaroraci/libnfc) para criar a sua cópia.
2. Clone a sua cópia e cadastre o repositório do projeto como um segundo remoto, aqui chamado `upstream`:

   ```
   $ git clone git@github.com:SEU_USUARIO/libnfc.git
   $ cd libnfc
   $ git remote add upstream https://github.com/icaroraci/libnfc.git
   ```

3. Antes de começar algo novo, traga a `main` do projeto para a sua:

   ```
   $ git switch main
   $ git pull --ff-only upstream main
   $ git push origin main
   ```

4. Trabalhe numa branch com nome descritivo, nunca direto na `main`:

   ```
   $ git switch -c qrcode-nfce
   ```

5. Faça os commits, rode os testes, envie a branch e abra o pull request pelo GitHub:

   ```
   $ git push -u origin qrcode-nfce
   ```

Se a `main` do projeto avançar enquanto o pull request está aberto, repita o passo 3 e faça `git merge main` na sua branch.
