# Dependências: libxml2 e OpenSSL (libcrypto), usadas diretamente, e
# libnfe 1.x (https://github.com/icaroraci/tooldoce), encontrada
# pelo pkg-config (libnfe.pc). Para usar uma libnfe sem libnfe.pc, informe
# as flags à mão:
#   make LIBNFE_CFLAGS="-I/opt/libnfe/include $(xml2-config --cflags)" \
#        LIBNFE_LIBS="-L/opt/libnfe/lib -lnfe"
PKG_CONFIG ?= pkg-config

ifeq ($(filter clean uninstall formatar verificar-formato,$(MAKECMDGOALS)),)
ifeq ($(origin LIBNFE_LIBS),undefined)
ifneq ($(shell $(PKG_CONFIG) --exists 'libnfe >= 1.0' 2>/dev/null && echo ok),ok)
$(error libnfe >= 1.0 não encontrada pelo pkg-config. Instale a libnfe (https://github.com/icaroraci/tooldoce), ajuste PKG_CONFIG_PATH ou informe LIBNFE_CFLAGS e LIBNFE_LIBS)
endif
endif
ifneq ($(shell $(PKG_CONFIG) --exists libxml-2.0 libcrypto 2>/dev/null && echo ok),ok)
$(error libxml2 ou OpenSSL (libcrypto) não encontrados. Instale as bibliotecas de desenvolvimento (ex.: apt install libxml2-dev libssl-dev))
endif
endif

LIBNFE_CFLAGS ?= $(shell $(PKG_CONFIG) --cflags libnfe 2>/dev/null)
LIBNFE_LIBS   ?= $(shell $(PKG_CONFIG) --libs libnfe 2>/dev/null)
#Diretório da libnfe, gravado como rpath nos testes e exemplos (vazio quando
#ela não vem do pkg-config)
LIBNFE_LIBDIR ?= $(shell $(PKG_CONFIG) --variable=libdir libnfe 2>/dev/null)
RPATH_LIBNFE   = $(if $(LIBNFE_LIBDIR),-Wl$(,)-rpath$(,)$(LIBNFE_LIBDIR))
, := ,


# Flags do compilador
# -MMD -MP gera arquivos .d para recompilar quando um header muda
CFLAGS := -Werror -Wall -Wextra -Wwrite-strings -std=c99 -g -fPIC -MMD -MP $(LIBNFE_CFLAGS) $(shell $(PKG_CONFIG) --cflags libxml-2.0 libcrypto 2>/dev/null)


# Flags para adicionar libs
LIBS := $(LIBNFE_LIBS) $(shell $(PKG_CONFIG) --libs libxml-2.0 libcrypto 2>/dev/null)


#-I includes
INCLUDE = ./include


#Paths do código fonte
SOURCE = ./src/libnfc


#Objetos compilados para Library
LOBJ = ./obj


#Path da lib
LIB = ./lib


#Versão da biblioteca, lida de include/libnfc/versao.h
versao = $(shell sed -n 's/^\#define NFC_VERSAO_$(1) *\([0-9a-z"]*\).*/\1/p' $(INCLUDE)/libnfc/versao.h)
VERSAO_MAIOR   := $(call versao,MAIOR)
VERSAO_MENOR   := $(call versao,MENOR)
VERSAO_REVISAO := $(call versao,REVISAO)
VERSAO_PRE     := $(subst ",,$(call versao,PRE))
VERSAO         := $(VERSAO_MAIOR).$(VERSAO_MENOR).$(VERSAO_REVISAO)$(if $(VERSAO_PRE),-$(VERSAO_PRE))


#Nomes da biblioteca compartilhada (o SONAME muda com a versão maior)
LIBNAME  = libnfc.so
SONAME   = $(LIBNAME).$(VERSAO_MAIOR)
REALNAME = $(SONAME).$(VERSAO_MENOR).$(VERSAO_REVISAO)


#Destino do `make install` (DESTDIR permite instalar em diretório temporário)
PREFIX        ?= /usr/local
LIBDIR        ?= $(PREFIX)/lib
INCLUDEDIR    ?= $(PREFIX)/include
PKGCONFIGDIR  ?= $(LIBDIR)/pkgconfig


#Nome de todas os arquivos fontes com path e extensão (*.c)
C_SOURCE = $(wildcard $(SOURCE)/*.c)


#Objetos com path ./obj/ e extensão (*.o)
OBJ = $(addprefix $(LOBJ)/,$(notdir $(C_SOURCE:.c=.o)))


.PHONY: all libnfc install uninstall test exemplos clean formatar verificar-formato

all: libnfc

libnfc: $(LIB)/$(REALNAME) $(LIB)/$(SONAME) $(LIB)/$(LIBNAME)

$(LIB)/$(REALNAME): $(OBJ) | $(LIB)
	$(CC) -shared -Wl,-soname,$(SONAME) $^ -o $@ $(LIBS)

#Links simbólicos: libnfc.so -> libnfc.so.1 -> libnfc.so.1.0.0
$(LIB)/$(SONAME): $(LIB)/$(REALNAME)
	ln -sf $(REALNAME) $@

$(LIB)/$(LIBNAME): $(LIB)/$(SONAME)
	ln -sf $(SONAME) $@

#Compila se não existir, ou recompila, se houve alteracao no fonte
$(LOBJ)/%.o: $(SOURCE)/%.c | $(LOBJ)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@


#Cria os diretórios de saída
$(LOBJ) $(LIB):
	mkdir -p $@


#Instala também o libnfc.pc, gerado com PREFIX, LIBDIR e INCLUDEDIR, para quem
#usa a biblioteca (pkg-config --cflags --libs libnfc)
install: libnfc
	install -d $(DESTDIR)$(LIBDIR) $(DESTDIR)$(INCLUDEDIR)/libnfc $(DESTDIR)$(PKGCONFIGDIR)
	install -m 755 $(LIB)/$(REALNAME) $(DESTDIR)$(LIBDIR)/
	ln -sf $(REALNAME) $(DESTDIR)$(LIBDIR)/$(SONAME)
	ln -sf $(SONAME) $(DESTDIR)$(LIBDIR)/$(LIBNAME)
	install -m 644 $(INCLUDE)/libnfc/*.h $(DESTDIR)$(INCLUDEDIR)/libnfc/
	sed -e 's|@PREFIX@|$(PREFIX)|' -e 's|@LIBDIR@|$(LIBDIR)|' \
	    -e 's|@INCLUDEDIR@|$(INCLUDEDIR)|' -e 's|@VERSAO@|$(VERSAO)|' \
	    libnfc.pc.in > $(DESTDIR)$(PKGCONFIGDIR)/libnfc.pc
	chmod 644 $(DESTDIR)$(PKGCONFIGDIR)/libnfc.pc


uninstall:
	rm -fv $(DESTDIR)$(LIBDIR)/$(LIBNAME) $(DESTDIR)$(LIBDIR)/$(SONAME) $(DESTDIR)$(LIBDIR)/$(REALNAME)
	rm -fv $(DESTDIR)$(PKGCONFIGDIR)/libnfc.pc
	rm -rfv $(DESTDIR)$(INCLUDEDIR)/libnfc


#Testes: cada tests/test_*.c vira um executável em obj/, compilado junto com
#os fontes da biblioteca e com sanitizers (desative com SANITIZE=)
TESTES = $(addprefix $(LOBJ)/,$(basename $(notdir $(wildcard tests/test_*.c))))
# O AddressSanitizer de compiladores mais antigos (ex.: GCC 12.2 do Debian 12)
# entra em laço ("AddressSanitizer:DEADLYSIGNAL") em kernels com
# vm.mmap_rnd_bits acima de 28, comum na WSL2; sem PIE os testes funcionam
# (tooldoce#254). Ao trocar SANITIZE, recompile os testes com make -B test.
MMAP_RND_BITS := $(shell cat /proc/sys/vm/mmap_rnd_bits 2>/dev/null)
SANITIZE_PIE := $(shell test "$(MMAP_RND_BITS)" -gt 28 2>/dev/null && echo "-fno-pie -no-pie")
SANITIZE ?= -fsanitize=address,undefined -fno-omit-frame-pointer $(SANITIZE_PIE)
CFLAGS_TESTE = $(filter-out -MMD -MP,$(CFLAGS)) $(SANITIZE)

test: $(TESTES)
	@for t in $(TESTES); do echo "== $$t"; $$t tests || exit 1; done

$(LOBJ)/test_%: tests/test_%.c $(wildcard tests/*.h) $(C_SOURCE) $(wildcard $(INCLUDE)/libnfc/*.h) | $(LOBJ)
	$(CC) $(CFLAGS_TESTE) -I$(INCLUDE) $< $(C_SOURCE) -o $@ $(LIBS) $(RPATH_LIBNFE)


#Exemplos: ligados à biblioteca compartilhada, como um programa externo
EXEMPLOS = $(addprefix $(LOBJ)/,$(basename $(notdir $(wildcard examples/*.c))))

exemplos: $(EXEMPLOS)

$(LOBJ)/%: examples/%.c $(LIB)/$(LIBNAME) | $(LOBJ)
	$(CC) $(filter-out -MMD -MP -fPIC,$(CFLAGS)) -I$(INCLUDE) $< -L$(LIB) -lnfc -Wl,-rpath,$(abspath $(LIB)) $(RPATH_LIBNFE) -o $@ $(LIBS)


#Formatação (.clang-format)
CLANG_FORMAT ?= clang-format
FONTES_C = $(wildcard $(SOURCE)/*.c $(INCLUDE)/libnfc/*.h tests/*.c tests/*.h examples/*.c)

formatar:
	$(CLANG_FORMAT) -i $(FONTES_C)

verificar-formato:
	$(CLANG_FORMAT) --dry-run --Werror $(FONTES_C)


clean:
	rm -fv $(LOBJ)/*.o $(LOBJ)/*.d $(LIB)/libnfc.so* $(TESTES) $(EXEMPLOS)


-include $(OBJ:.o=.d)
