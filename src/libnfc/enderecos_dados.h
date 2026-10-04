/* Gerado por tools/gerar_enderecos.py; não edite. */
/* Fontes e data de captura: arquivos .tsv em docs/enderecos. */

enum { AUTOR_AM, AUTOR_GO, AUTOR_MG, AUTOR_MS, AUTOR_MT, AUTOR_PR, AUTOR_RS, AUTOR_SP, AUTOR_SVRS, AUTOR_TOTAL };

/* Ambiente: [0] produção, [1] homologação */
static const char *const webservices[2][AUTOR_TOTAL][NFE_SERVICO_INUTILIZACAO + 1] = {
	[0] = {
		[AUTOR_AM] = {
			[NFE_SERVICO_AUTORIZACAO] =
				"https://nfce.sefaz.am.gov.br/nfce-services/services/NfeAutor"
				"izacao4",
			[NFE_SERVICO_RET_AUTORIZACAO] =
				"https://nfce.sefaz.am.gov.br/nfce-services/services/NfeRetAu"
				"torizacao4",
			[NFE_SERVICO_CONSULTA] =
				"https://nfce.sefaz.am.gov.br/nfce-services/services/NfeConsu"
				"lta4",
			[NFE_SERVICO_STATUS] =
				"https://nfce.sefaz.am.gov.br/nfce-services/services/NfeStatu"
				"sServico4",
			[NFE_SERVICO_EVENTO] =
				"https://nfce.sefaz.am.gov.br/nfce-services/services/Recepcao"
				"Evento4",
			[NFE_SERVICO_INUTILIZACAO] =
				"https://nfce.sefaz.am.gov.br/nfce-services/services/NfeInuti"
				"lizacao4",
		},
		[AUTOR_GO] = {
			[NFE_SERVICO_AUTORIZACAO] =
				"https://nfe.sefaz.go.gov.br/nfe/services/NFeAutorizacao4",
			[NFE_SERVICO_RET_AUTORIZACAO] =
				"https://nfe.sefaz.go.gov.br/nfe/services/NFeRetAutorizacao4",
			[NFE_SERVICO_CONSULTA] =
				"https://nfe.sefaz.go.gov.br/nfe/services/NFeConsultaProtocol"
				"o4",
			[NFE_SERVICO_STATUS] =
				"https://nfe.sefaz.go.gov.br/nfe/services/NFeStatusServico4",
			[NFE_SERVICO_EVENTO] =
				"https://nfe.sefaz.go.gov.br/nfe/services/NFeRecepcaoEvento4",
			[NFE_SERVICO_INUTILIZACAO] =
				"https://nfe.sefaz.go.gov.br/nfe/services/NFeInutilizacao4",
		},
		[AUTOR_MG] = {
			[NFE_SERVICO_AUTORIZACAO] =
				"https://nfce.fazenda.mg.gov.br/nfce/services/NFeAutorizacao4",
			[NFE_SERVICO_RET_AUTORIZACAO] =
				"https://nfce.fazenda.mg.gov.br/nfce/services/NFeRetAutorizac"
				"ao4",
			[NFE_SERVICO_CONSULTA] =
				"https://nfce.fazenda.mg.gov.br/nfce/services/NFeConsultaProt"
				"ocolo4",
			[NFE_SERVICO_STATUS] =
				"https://nfce.fazenda.mg.gov.br/nfce/services/NFeStatusServic"
				"o4",
			[NFE_SERVICO_EVENTO] =
				"https://nfce.fazenda.mg.gov.br/nfce/services/NFeRecepcaoEven"
				"to4",
			[NFE_SERVICO_INUTILIZACAO] =
				"https://nfce.fazenda.mg.gov.br/nfce/services/NFeInutilizacao"
				"4",
		},
		[AUTOR_MS] = {
			[NFE_SERVICO_AUTORIZACAO] =
				"https://nfce.sefaz.ms.gov.br/ws/NFeAutorizacao4",
			[NFE_SERVICO_RET_AUTORIZACAO] =
				"https://nfce.sefaz.ms.gov.br/ws/NFeRetAutorizacao4",
			[NFE_SERVICO_CONSULTA] =
				"https://nfce.sefaz.ms.gov.br/ws/NFeConsultaProtocolo4",
			[NFE_SERVICO_STATUS] =
				"https://nfce.sefaz.ms.gov.br/ws/NFeStatusServico4",
			[NFE_SERVICO_EVENTO] =
				"https://nfce.sefaz.ms.gov.br/ws/NFeRecepcaoEvento4",
			[NFE_SERVICO_INUTILIZACAO] =
				"https://nfce.sefaz.ms.gov.br/ws/NFeInutilizacao4",
		},
		[AUTOR_MT] = {
			[NFE_SERVICO_AUTORIZACAO] =
				"https://nfce.sefaz.mt.gov.br/nfcews/services/NfeAutorizacao4",
			[NFE_SERVICO_RET_AUTORIZACAO] =
				"https://nfce.sefaz.mt.gov.br/nfcews/services/NfeRetAutorizac"
				"ao4",
			[NFE_SERVICO_CONSULTA] =
				"https://nfce.sefaz.mt.gov.br/nfcews/services/NfeConsulta4",
			[NFE_SERVICO_STATUS] =
				"https://nfce.sefaz.mt.gov.br/nfcews/services/NfeStatusServic"
				"o4",
			[NFE_SERVICO_EVENTO] =
				"https://nfce.sefaz.mt.gov.br/nfcews/services/RecepcaoEvento4",
			[NFE_SERVICO_INUTILIZACAO] =
				"https://nfce.sefaz.mt.gov.br/nfcews/services/NfeInutilizacao"
				"4",
		},
		[AUTOR_PR] = {
			[NFE_SERVICO_AUTORIZACAO] =
				"https://nfce.sefa.pr.gov.br/nfce/NFeAutorizacao4",
			[NFE_SERVICO_RET_AUTORIZACAO] =
				"https://nfce.sefa.pr.gov.br/nfce/NFeRetAutorizacao4",
			[NFE_SERVICO_CONSULTA] =
				"https://nfce.sefa.pr.gov.br/nfce/NFeConsultaProtocolo4",
			[NFE_SERVICO_STATUS] =
				"https://nfce.sefa.pr.gov.br/nfce/NFeStatusServico4",
			[NFE_SERVICO_EVENTO] =
				"https://nfce.sefa.pr.gov.br/nfce/NFeRecepcaoEvento4",
			[NFE_SERVICO_INUTILIZACAO] =
				"https://nfce.sefa.pr.gov.br/nfce/NFeInutilizacao4",
		},
		[AUTOR_RS] = {
			[NFE_SERVICO_AUTORIZACAO] =
				"https://nfce.sefazrs.rs.gov.br/ws/NfeAutorizacao/NFeAutoriza"
				"cao4.asmx",
			[NFE_SERVICO_RET_AUTORIZACAO] =
				"https://nfce.sefazrs.rs.gov.br/ws/NfeRetAutorizacao/NFeRetAu"
				"torizacao4.asmx",
			[NFE_SERVICO_CONSULTA] =
				"https://nfce.sefazrs.rs.gov.br/ws/NfeConsulta/NfeConsulta4.a"
				"smx",
			[NFE_SERVICO_STATUS] =
				"https://nfce.sefazrs.rs.gov.br/ws/NfeStatusServico/NfeStatus"
				"Servico4.asmx",
			[NFE_SERVICO_EVENTO] =
				"https://nfce.sefazrs.rs.gov.br/ws/recepcaoevento/recepcaoeve"
				"nto4.asmx",
			[NFE_SERVICO_INUTILIZACAO] =
				"https://nfce.sefazrs.rs.gov.br/ws/nfeinutilizacao/nfeinutili"
				"zacao4.asmx",
		},
		[AUTOR_SP] = {
			[NFE_SERVICO_AUTORIZACAO] =
				"https://nfce.fazenda.sp.gov.br/ws/NFeAutorizacao4.asmx",
			[NFE_SERVICO_RET_AUTORIZACAO] =
				"https://nfce.fazenda.sp.gov.br/ws/NFeRetAutorizacao4.asmx",
			[NFE_SERVICO_CONSULTA] =
				"https://nfce.fazenda.sp.gov.br/ws/NFeConsultaProtocolo4.asmx",
			[NFE_SERVICO_STATUS] =
				"https://nfce.fazenda.sp.gov.br/ws/NFeStatusServico4.asmx",
			[NFE_SERVICO_EVENTO] =
				"https://nfce.fazenda.sp.gov.br/ws/NFeRecepcaoEvento4.asmx",
			[NFE_SERVICO_INUTILIZACAO] =
				"https://nfce.fazenda.sp.gov.br/ws/NFeInutilizacao4.asmx",
		},
		[AUTOR_SVRS] = {
			[NFE_SERVICO_AUTORIZACAO] =
				"https://nfce.svrs.rs.gov.br/ws/NfeAutorizacao/NFeAutorizacao"
				"4.asmx",
			[NFE_SERVICO_RET_AUTORIZACAO] =
				"https://nfce.svrs.rs.gov.br/ws/NfeRetAutorizacao/NFeRetAutor"
				"izacao4.asmx",
			[NFE_SERVICO_CONSULTA] =
				"https://nfce.svrs.rs.gov.br/ws/NfeConsulta/NfeConsulta4.asmx",
			[NFE_SERVICO_STATUS] =
				"https://nfce.svrs.rs.gov.br/ws/NfeStatusServico/NfeStatusSer"
				"vico4.asmx",
			[NFE_SERVICO_EVENTO] =
				"https://nfce.svrs.rs.gov.br/ws/recepcaoevento/recepcaoevento"
				"4.asmx",
			[NFE_SERVICO_INUTILIZACAO] =
				"https://nfce.svrs.rs.gov.br/ws/nfeinutilizacao/nfeinutilizac"
				"ao4.asmx",
		},
	},
	[1] = {
		[AUTOR_AM] = {
			[NFE_SERVICO_AUTORIZACAO] =
				"https://homnfce.sefaz.am.gov.br/nfce-services/services/NfeAu"
				"torizacao4",
			[NFE_SERVICO_RET_AUTORIZACAO] =
				"https://homnfce.sefaz.am.gov.br/nfce-services/services/NfeRe"
				"tAutorizacao4",
			[NFE_SERVICO_CONSULTA] =
				"https://homnfce.sefaz.am.gov.br/nfce-services/services/NfeCo"
				"nsulta4",
			[NFE_SERVICO_STATUS] =
				"https://homnfce.sefaz.am.gov.br/nfce-services/services/NfeSt"
				"atusServico4",
			[NFE_SERVICO_EVENTO] =
				"https://homnfce.sefaz.am.gov.br/nfce-services/services/Recep"
				"caoEvento4",
			[NFE_SERVICO_INUTILIZACAO] =
				"https://homnfce.sefaz.am.gov.br/nfce-services/services/NfeIn"
				"utilizacao4",
		},
		[AUTOR_GO] = {
			[NFE_SERVICO_AUTORIZACAO] =
				"https://homolog.sefaz.go.gov.br/nfe/services/NFeAutorizacao4",
			[NFE_SERVICO_RET_AUTORIZACAO] =
				"https://homolog.sefaz.go.gov.br/nfe/services/NFeRetAutorizac"
				"ao4",
			[NFE_SERVICO_CONSULTA] =
				"https://homolog.sefaz.go.gov.br/nfe/services/NFeConsultaProt"
				"ocolo4",
			[NFE_SERVICO_STATUS] =
				"https://homolog.sefaz.go.gov.br/nfe/services/NFeStatusServic"
				"o4",
			[NFE_SERVICO_EVENTO] =
				"https://homolog.sefaz.go.gov.br/nfe/services/NFeRecepcaoEven"
				"to4",
			[NFE_SERVICO_INUTILIZACAO] =
				"https://homolog.sefaz.go.gov.br/nfe/services/NFeInutilizacao"
				"4",
		},
		[AUTOR_MG] = {
			[NFE_SERVICO_AUTORIZACAO] =
				"https://hnfce.fazenda.mg.gov.br/nfce/services/NFeAutorizacao"
				"4",
			[NFE_SERVICO_RET_AUTORIZACAO] =
				"https://hnfce.fazenda.mg.gov.br/nfce/services/NFeRetAutoriza"
				"cao4",
			[NFE_SERVICO_CONSULTA] =
				"https://hnfce.fazenda.mg.gov.br/nfce/services/NFeConsultaPro"
				"tocolo4",
			[NFE_SERVICO_STATUS] =
				"https://hnfce.fazenda.mg.gov.br/nfce/services/NFeStatusServi"
				"co4",
			[NFE_SERVICO_EVENTO] =
				"https://hnfce.fazenda.mg.gov.br/nfce/services/NFeRecepcaoEve"
				"nto4",
			[NFE_SERVICO_INUTILIZACAO] =
				"https://hnfce.fazenda.mg.gov.br/nfce/services/NFeInutilizaca"
				"o4",
		},
		[AUTOR_MS] = {
			[NFE_SERVICO_AUTORIZACAO] =
				"https://hom.nfce.sefaz.ms.gov.br/ws/NFeAutorizacao4",
			[NFE_SERVICO_RET_AUTORIZACAO] =
				"https://hom.nfce.sefaz.ms.gov.br/ws/NFeRetAutorizacao4",
			[NFE_SERVICO_CONSULTA] =
				"https://hom.nfce.sefaz.ms.gov.br/ws/NFeConsultaProtocolo4",
			[NFE_SERVICO_STATUS] =
				"https://hom.nfce.sefaz.ms.gov.br/ws/NFeStatusServico4",
			[NFE_SERVICO_EVENTO] =
				"https://hom.nfce.sefaz.ms.gov.br/ws/NFeRecepcaoEvento4",
			[NFE_SERVICO_INUTILIZACAO] =
				"https://hom.nfce.sefaz.ms.gov.br/ws/NFeInutilizacao4",
		},
		[AUTOR_MT] = {
			[NFE_SERVICO_AUTORIZACAO] =
				"https://homologacao.sefaz.mt.gov.br/nfcews/services/NfeAutor"
				"izacao4",
			[NFE_SERVICO_RET_AUTORIZACAO] =
				"https://homologacao.sefaz.mt.gov.br/nfcews/services/NfeRetAu"
				"torizacao4",
			[NFE_SERVICO_CONSULTA] =
				"https://homologacao.sefaz.mt.gov.br/nfcews/services/NfeConsu"
				"lta4",
			[NFE_SERVICO_STATUS] =
				"https://homologacao.sefaz.mt.gov.br/nfcews/services/NfeStatu"
				"sServico4",
			[NFE_SERVICO_EVENTO] =
				"https://homologacao.sefaz.mt.gov.br/nfcews/services/Recepcao"
				"Evento4",
			[NFE_SERVICO_INUTILIZACAO] =
				"https://homologacao.sefaz.mt.gov.br/nfcews/services/NfeInuti"
				"lizacao4",
		},
		[AUTOR_PR] = {
			[NFE_SERVICO_AUTORIZACAO] =
				"https://homologacao.nfce.sefa.pr.gov.br/nfce/NFeAutorizacao4",
			[NFE_SERVICO_RET_AUTORIZACAO] =
				"https://homologacao.nfce.sefa.pr.gov.br/nfce/NFeRetAutorizac"
				"ao4",
			[NFE_SERVICO_CONSULTA] =
				"https://homologacao.nfce.sefa.pr.gov.br/nfce/NFeConsultaProt"
				"ocolo4",
			[NFE_SERVICO_STATUS] =
				"https://homologacao.nfce.sefa.pr.gov.br/nfce/NFeStatusServic"
				"o4",
			[NFE_SERVICO_EVENTO] =
				"https://homologacao.nfce.sefa.pr.gov.br/nfce/NFeRecepcaoEven"
				"to4",
			[NFE_SERVICO_INUTILIZACAO] =
				"https://homologacao.nfce.sefa.pr.gov.br/nfce/NFeInutilizacao"
				"4",
		},
		[AUTOR_RS] = {
			[NFE_SERVICO_AUTORIZACAO] =
				"https://nfce-homologacao.sefazrs.rs.gov.br/ws/NfeAutorizacao"
				"/NFeAutorizacao4.asmx",
			[NFE_SERVICO_RET_AUTORIZACAO] =
				"https://nfce-homologacao.sefazrs.rs.gov.br/ws/NfeRetAutoriza"
				"cao/NFeRetAutorizacao4.asmx",
			[NFE_SERVICO_CONSULTA] =
				"https://nfce-homologacao.sefazrs.rs.gov.br/ws/NfeConsulta/Nf"
				"eConsulta4.asmx",
			[NFE_SERVICO_STATUS] =
				"https://nfce-homologacao.sefazrs.rs.gov.br/ws/NfeStatusServi"
				"co/NfeStatusServico4.asmx",
			[NFE_SERVICO_EVENTO] =
				"https://nfce-homologacao.sefazrs.rs.gov.br/ws/recepcaoevento"
				"/recepcaoevento4.asmx",
			[NFE_SERVICO_INUTILIZACAO] =
				"https://nfce-homologacao.sefazrs.rs.gov.br/ws/nfeinutilizaca"
				"o/nfeinutilizacao4.asmx",
		},
		[AUTOR_SP] = {
			[NFE_SERVICO_AUTORIZACAO] =
				"https://homologacao.nfce.fazenda.sp.gov.br/ws/NFeAutorizacao"
				"4.asmx",
			[NFE_SERVICO_RET_AUTORIZACAO] =
				"https://homologacao.nfce.fazenda.sp.gov.br/ws/NFeRetAutoriza"
				"cao4.asmx",
			[NFE_SERVICO_CONSULTA] =
				"https://homologacao.nfce.fazenda.sp.gov.br/ws/NFeConsultaPro"
				"tocolo4.asmx",
			[NFE_SERVICO_STATUS] =
				"https://homologacao.nfce.fazenda.sp.gov.br/ws/NFeStatusServi"
				"co4.asmx",
			[NFE_SERVICO_EVENTO] =
				"https://homologacao.nfce.fazenda.sp.gov.br/ws/NFeRecepcaoEve"
				"nto4.asmx",
			[NFE_SERVICO_INUTILIZACAO] =
				"https://homologacao.nfce.fazenda.sp.gov.br/ws/NFeInutilizaca"
				"o4.asmx",
		},
		[AUTOR_SVRS] = {
			[NFE_SERVICO_AUTORIZACAO] =
				"https://nfce-homologacao.svrs.rs.gov.br/ws/NfeAutorizacao/NF"
				"eAutorizacao4.asmx",
			[NFE_SERVICO_RET_AUTORIZACAO] =
				"https://nfce-homologacao.svrs.rs.gov.br/ws/NfeRetAutorizacao"
				"/NFeRetAutorizacao4.asmx",
			[NFE_SERVICO_CONSULTA] =
				"https://nfce-homologacao.svrs.rs.gov.br/ws/NfeConsulta/NfeCo"
				"nsulta4.asmx",
			[NFE_SERVICO_STATUS] =
				"https://nfce-homologacao.svrs.rs.gov.br/ws/NfeStatusServico/"
				"NfeStatusServico4.asmx",
			[NFE_SERVICO_EVENTO] =
				"https://nfce-homologacao.svrs.rs.gov.br/ws/recepcaoevento/re"
				"cepcaoevento4.asmx",
			[NFE_SERVICO_INUTILIZACAO] =
				"https://nfce-homologacao.svrs.rs.gov.br/ws/nfeinutilizacao/n"
				"feinutilizacao4.asmx",
		},
	},
};

static const struct {
	nfe_uf uf;
	int autor;
	const char *qrcode[2], *chave[2];
} ufs[] = {
	{ NFE_UF_RO, AUTOR_SVRS,
	  { "http://www.nfce.sefin.ro.gov.br/consultanfce/consulta.jsp",
	    "http://www.nfce.sefin.ro.gov.br/consultanfce/consulta.jsp" },
	  { "www.sefin.ro.gov.br/nfce/consulta",
	    "www.sefin.ro.gov.br/nfce/consulta" } },
	{ NFE_UF_AC, AUTOR_SVRS,
	  { "http://www.sefaznet.ac.gov.br/nfce/qrcode",
	    "http://www.hml.sefaznet.ac.gov.br/nfce/qrcode" },
	  { "www.sefaznet.ac.gov.br/nfce/consulta",
	    "www.sefaznet.ac.gov.br/nfce/consulta" } },
	{ NFE_UF_AM, AUTOR_AM,
	  { "http://sistemas.sefaz.am.gov.br/nfceweb/consultarNFCe.jsp",
	    "http://homnfce.sefaz.am.gov.br/nfceweb/consultarNFCe.jsp" },
	  { "www.sefaz.am.gov.br/nfce/consulta",
	    "www.sefaz.am.gov.br/nfce/consulta" } },
	{ NFE_UF_RR, AUTOR_SVRS,
	  { "https://www.sefaz.rr.gov.br/nfce/servlet/qrcode",
	    "http://200.174.88.103:8080/nfce/servlet/qrcode" },
	  { "www.sefaz.rr.gov.br/nfce/consulta",
	    "www.sefaz.rr.gov.br/nfce/consulta" } },
	{ NFE_UF_PA, AUTOR_SVRS,
	  { "https://appnfc.sefa.pa.gov.br/portal/view/consultas/nfce/nfc"
	    "eForm.seam",
	    "https://appnfc.sefa.pa.gov.br/portal-homologacao/view/consul"
	    "tas/nfce/nfceForm.seam" },
	  { "www.sefa.pa.gov.br/nfce/consulta",
	    "www.sefa.pa.gov.br/nfce/consulta" } },
	{ NFE_UF_AP, AUTOR_SVRS,
	  { "https://www.sefaz.ap.gov.br/nfce/nfcep.php",
	    "https://www.sefaz.ap.gov.br/nfcehml/nfce.php" },
	  { "www.sefaz.ap.gov.br/nfce/consulta",
	    "www.sefaz.ap.gov.br/nfce/consulta" } },
	{ NFE_UF_TO, AUTOR_SVRS,
	  { "http://www.sefaz.to.gov.br/nfce/qrcode",
	    "http://homologacao.sefaz.to.gov.br/nfce/qrcode" },
	  { "www.sefaz.to.gov.br/nfce/consulta",
	    "http://homologacao.sefaz.to.gov.br/nfce/consulta.jsf" } },
	{ NFE_UF_MA, AUTOR_SVRS,
	  { "http://nfce.sefaz.ma.gov.br/portal/consultarNFCe.jsp",
	    "http://homologacao.sefaz.ma.gov.br/portal/consultarNFCe.jsp" },
	  { "www.sefaz.ma.gov.br/nfce/consulta",
	    "www.sefaz.ma.gov.br/nfce/consulta" } },
	{ NFE_UF_PI, AUTOR_SVRS,
	  { "http://www.sefaz.pi.gov.br/nfce/qrcode",
	    "http://www.sefaz.pi.gov.br/nfce/qrcode" },
	  { "www.sefaz.pi.gov.br/nfce/consulta",
	    "www.sefaz.pi.gov.br/nfce/consulta" } },
	{ NFE_UF_CE, AUTOR_SVRS,
	  { "http://nfce.sefaz.ce.gov.br/pages/ShowNFCe.html",
	    "http://nfceh.sefaz.ce.gov.br/pages/ShowNFCe.html" },
	  { "www.sefaz.ce.gov.br/nfce/consulta",
	    "www.sefaz.ce.gov.br/nfce/consulta" } },
	{ NFE_UF_RN, AUTOR_SVRS,
	  { "https://nfce.sefaz.rn.gov.br/consultarNFCe.aspx",
	    "https://hom.nfce.sefaz.rn.gov.br/consultarNFCe.aspx" },
	  { "www.set.rn.gov.br/nfce/consulta",
	    "www.set.rn.gov.br/nfce/consulta" } },
	{ NFE_UF_PB, AUTOR_SVRS,
	  { "http://www.sefaz.pb.gov.br/nfce",
	    "http://www.sefaz.pb.gov.br/nfcehom" },
	  { "www.sefaz.pb.gov.br/nfce/consulta",
	    "www.sefaz.pb.gov.br/nfcehom" } },
	{ NFE_UF_PE, AUTOR_SVRS,
	  { "http://nfce.sefaz.pe.gov.br/nfce/consulta",
	    "http://nfcehomolog.sefaz.pe.gov.br/nfce/consulta" },
	  { "nfce.sefaz.pe.gov.br/nfce/consulta",
	    "nfce.sefaz.pe.gov.br/nfce/consulta" } },
	{ NFE_UF_AL, AUTOR_SVRS,
	  { "http://nfce.sefaz.al.gov.br/QRCode/consultarNFCe.jsp",
	    "http://nfce.sefaz.al.gov.br/QRCode/consultarNFCe.jsp" },
	  { "www.sefaz.al.gov.br/nfce/consulta",
	    "www.sefaz.al.gov.br/nfce/consulta" } },
	{ NFE_UF_SE, AUTOR_SVRS,
	  { "http://www.nfce.se.gov.br/nfce/qrcode",
	    "http://www.hom.nfe.se.gov.br/nfce/qrcode" },
	  { "http://www.nfce.se.gov.br/nfce/consulta",
	    "http://www.hom.nfe.se.gov.br/nfce/consulta" } },
	{ NFE_UF_BA, AUTOR_SVRS,
	  { "http://nfe.sefaz.ba.gov.br/servicos/nfce/qrcode.aspx",
	    "http://hnfe.sefaz.ba.gov.br/servicos/nfce/qrcode.aspx" },
	  { "www.sefaz.ba.gov.br/nfce/consulta",
	    "http://hinternet.sefaz.ba.gov.br/nfce/consulta" } },
	{ NFE_UF_MG, AUTOR_MG,
	  { "https://portalsped.fazenda.mg.gov.br/portalnfce/sistema/qrco"
	    "de.xhtml",
	    "https://portalsped.fazenda.mg.gov.br/portalnfce/sistema/qrco"
	    "de.xhtml" },
	  { "http://nfce.fazenda.mg.gov.br/portalnfce",
	    "http://hnfce.fazenda.mg.gov.br/portalnfce/" } },
	{ NFE_UF_ES, AUTOR_SVRS,
	  { "http://app.sefaz.es.gov.br/ConsultaNFCe/",
	    "http://homologacao.sefaz.es.gov.br/ConsultaNFCe/" },
	  { "www.sefaz.es.gov.br/nfce/consulta",
	    "www.sefaz.es.gov.br/nfce/consulta" } },
	{ NFE_UF_RJ, AUTOR_SVRS,
	  { "https://consultadfe.fazenda.rj.gov.br/consultaNFCe/QRCode",
	    "https://consultadfe.fazenda.rj.gov.br/consultaNFCe/QRCode" },
	  { "www.fazenda.rj.gov.br/nfce/consulta",
	    "www.fazenda.rj.gov.br/nfce/consulta" } },
	{ NFE_UF_SP, AUTOR_SP,
	  { "https://www.nfce.fazenda.sp.gov.br/qrcode",
	    "https://www.homologacao.nfce.fazenda.sp.gov.br/qrcode" },
	  { "https://www.nfce.fazenda.sp.gov.br/consulta",
	    "https://www.homologacao.nfce.fazenda.sp.gov.br/consulta" } },
	{ NFE_UF_PR, AUTOR_PR,
	  { "http://www.fazenda.pr.gov.br/nfce/qrcode",
	    "http://www.fazenda.pr.gov.br/nfce/qrcode" },
	  { "http://www.fazenda.pr.gov.br/nfce/consulta",
	    "http://www.fazenda.pr.gov.br/nfce/consulta" } },
	{ NFE_UF_SC, AUTOR_SVRS,
	  { "https://sat.sef.sc.gov.br/nfce/consulta",
	    "https://hom.sat.sef.sc.gov.br/nfce/consulta" },
	  { "https://sat.sef.sc.gov.br/nfce/consulta",
	    "https://hom.sat.sef.sc.gov.br/nfce/consulta" } },
	{ NFE_UF_RS, AUTOR_RS,
	  { "https://www.sefaz.rs.gov.br/NFCE/NFCE-COM.aspx",
	    "https://www.sefaz.rs.gov.br/NFCE/NFCE-COM.aspx" },
	  { "www.sefaz.rs.gov.br/nfce/consulta",
	    "www.sefaz.rs.gov.br/nfce/consulta" } },
	{ NFE_UF_MS, AUTOR_MS,
	  { "http://www.dfe.ms.gov.br/nfce/qrcode",
	    "http://www.dfe.ms.gov.br/nfce/qrcode" },
	  { "www.dfe.ms.gov.br/nfce/consulta",
	    "www.dfe.ms.gov.br/nfce/consulta" } },
	{ NFE_UF_MT, AUTOR_MT,
	  { "http://www.sefaz.mt.gov.br/nfce/consultanfce",
	    "http://homologacao.sefaz.mt.gov.br/nfce/consultanfce" },
	  { "http://www.sefaz.mt.gov.br/nfce/consultanfce",
	    "http://homologacao.sefaz.mt.gov.br/nfce/consultanfce" } },
	{ NFE_UF_GO, AUTOR_GO,
	  { "https://nfeweb.sefaz.go.gov.br/nfeweb/sites/nfce/danfeNFCe",
	    "https://nfewebhomolog.sefaz.go.gov.br/nfeweb/sites/nfce/danf"
	    "eNFCe" },
	  { "www.sefaz.go.gov.br/nfce/consulta",
	    "www.sefaz.go.gov.br/nfce/consulta" } },
	{ NFE_UF_DF, AUTOR_SVRS,
	  { "http://www.fazenda.df.gov.br/nfce/qrcode",
	    "http://www.fazenda.df.gov.br/nfce/qrcode" },
	  { "www.fazenda.df.gov.br/nfce/consulta",
	    "www.fazenda.df.gov.br/nfce/consulta" } },
};
