#!/usr/bin/env python3
# Gera src/libnfc/enderecos_dados.h a partir de docs/enderecos/*.tsv.
#
# Uso (na raiz do projeto): python3 tools/gerar_enderecos.py
#
# Os .tsv registram a fonte e a data de captura de cada tabela; ao atualizar
# um endereço, mude o .tsv (com a fonte) e rode este script de novo.

import os
import sys

RAIZ = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DOCS = os.path.join(RAIZ, 'docs', 'enderecos')
SAIDA = os.path.join(RAIZ, 'src', 'libnfc', 'enderecos_dados.h')

UFS = ['RO', 'AC', 'AM', 'RR', 'PA', 'AP', 'TO', 'MA', 'PI', 'CE', 'RN',
       'PB', 'PE', 'AL', 'SE', 'BA', 'MG', 'ES', 'RJ', 'SP', 'PR', 'SC',
       'RS', 'MS', 'MT', 'GO', 'DF']
SERVICOS = {
    'autorizacao': 'NFE_SERVICO_AUTORIZACAO',
    'ret_autorizacao': 'NFE_SERVICO_RET_AUTORIZACAO',
    'consulta': 'NFE_SERVICO_CONSULTA',
    'status': 'NFE_SERVICO_STATUS',
    'evento': 'NFE_SERVICO_EVENTO',
    'inutilizacao': 'NFE_SERVICO_INUTILIZACAO',
}
# Índice do ambiente nas tabelas: 0 produção, 1 homologação
AMB = {'p': 0, 'h': 1}


def linhas(nome, colunas):
    caminho = os.path.join(DOCS, nome)
    with open(caminho, encoding='utf-8') as f:
        for n, linha in enumerate(f, 1):
            linha = linha.rstrip('\n')
            if not linha or linha.startswith('#'):
                continue
            campos = linha.split('\t')
            if len(campos) != colunas:
                sys.exit('%s:%d: esperadas %d colunas' % (nome, n, colunas))
            yield campos


def confere_url(url, onde):
    if any(c.isspace() for c in url) or '"' in url or '\\' in url:
        sys.exit('%s: URL inválida: %r' % (onde, url))


def literal(s, recuo):
    # Quebra a string em pedaços que cabem na linha
    partes = [s[i:i + 60] for i in range(0, len(s), 60)]
    return ('\n' + recuo).join('"%s"' % p for p in partes)


def main():
    ws = {}
    for autor, amb, serv, url in linhas('webservices.tsv', 4):
        if amb not in AMB or serv not in SERVICOS:
            sys.exit('webservices.tsv: %s %s %s' % (autor, amb, serv))
        if not url.startswith('https://') or '?' in url:
            sys.exit('webservices.tsv: URL inválida: %s' % url)
        confere_url(url, 'webservices.tsv')
        ws.setdefault(autor, {})[(AMB[amb], serv)] = url
    autores = sorted(ws)
    for autor in autores:
        for amb in AMB.values():
            for serv in SERVICOS:
                if (amb, serv) not in ws[autor]:
                    sys.exit('webservices.tsv: falta %s %d %s' %
                             (autor, amb, serv))

    autorizador = {}
    for uf, autor in linhas('autorizadores.tsv', 2):
        if uf not in UFS or autor not in ws:
            sys.exit('autorizadores.tsv: %s %s' % (uf, autor))
        autorizador[uf] = autor

    consulta = {}
    for uf, amb, qr, chave in linhas('consulta.tsv', 4):
        if uf not in UFS or amb not in AMB:
            sys.exit('consulta.tsv: %s %s' % (uf, amb))
        if not qr.startswith(('http://', 'https://')) or '?' in qr:
            sys.exit('consulta.tsv: qrCode inválido: %s' % qr)
        if not 21 <= len(chave) <= 85:
            sys.exit('consulta.tsv: urlChave fora de 21 a 85: %s' % chave)
        confere_url(qr, 'consulta.tsv')
        confere_url(chave, 'consulta.tsv')
        consulta[(uf, AMB[amb])] = (qr, chave)

    for uf in UFS:
        if uf not in autorizador:
            sys.exit('autorizadores.tsv: falta %s' % uf)
        for amb in AMB.values():
            if (uf, amb) not in consulta:
                sys.exit('consulta.tsv: falta %s %d' % (uf, amb))

    out = []
    out.append('/* Gerado por tools/gerar_enderecos.py; não edite. */')
    out.append('/* Fontes e data de captura: arquivos .tsv em docs/enderecos. */')
    out.append('')
    out.append('enum { %s, AUTOR_TOTAL };' %
               ', '.join('AUTOR_%s' % a for a in autores))
    out.append('')
    out.append('/* Ambiente: [0] produção, [1] homologação */')
    out.append('static const char *const webservices[2][AUTOR_TOTAL]'
               '[NFE_SERVICO_INUTILIZACAO + 1] = {')
    for amb in (0, 1):
        out.append('\t[%d] = {' % amb)
        for autor in autores:
            out.append('\t\t[AUTOR_%s] = {' % autor)
            for serv, const in SERVICOS.items():
                out.append('\t\t\t[%s] =' % const)
                out.append('\t\t\t\t' + literal(ws[autor][(amb, serv)],
                                                 '\t\t\t\t') + ',')
            out.append('\t\t},')
        out.append('\t},')
    out.append('};')
    out.append('')
    out.append('static const struct {')
    out.append('\tnfe_uf uf;')
    out.append('\tint autor;')
    out.append('\tconst char *qrcode[2], *chave[2];')
    out.append('} ufs[] = {')
    for uf in UFS:
        out.append('\t{ NFE_UF_%s, AUTOR_%s,' % (uf, autorizador[uf]))
        for nome, idx in (('qrcode', 0), ('chave', 1)):
            vals = [consulta[(uf, amb)][idx] for amb in (0, 1)]
            out.append('\t  { ' + literal(vals[0], '\t    ') + ',')
            out.append('\t    ' + literal(vals[1], '\t    ') +
                       (' } },' if nome == 'chave' else ' },'))
    out.append('};')
    texto = '\n'.join(out) + '\n'
    with open(SAIDA, 'w', encoding='utf-8') as f:
        f.write(texto)
    print('gravado %s' % os.path.relpath(SAIDA, RAIZ))


if __name__ == '__main__':
    main()
