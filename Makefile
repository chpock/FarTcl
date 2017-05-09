NAME = FarTcl
DLLNAME = $(NAME).dll
INSTALL_PREFIX = /cygdrive/c/Far Manager/Plugins/FarTcl
DEBUG = 1

CC=i686-w64-mingw32-gcc
RC=i686-w64-mingw32-windres
AR=i686-w64-mingw32-ar
RUNLIB=i686-w64-mingw32-ranlib
CPP=$(CC)

ifndef DEBUG
_ODN_=final
else
_ODN_=debug
endif
ifdef AMD64
_ODB_=x86-64
else
_ODB_=x86
endif

BLDDIR := $(shell pwd)

TCLVER       = 86
TCLDIR       = $(BLDDIR)/src/tcl
TCLSH        = $(TCLDIR)/win/tclsh$(TCLVER).exe
TCLCONFFLAGS = CC=$(CC) AR=$(AR) RC=$(RC) RUNLIB=$(RUNLIB) CFLAGS=-m32
TCLSTUB      = $(TCLDIR)/win/libtcl$(TCLVER).$(LIBEXT)

OUTDIR = $(BLDDIR)/out.$(_ODN_).$(_ODB_)

OBJDIR = $(OUTDIR)/obj
INCLUDE = include:$(TCLDIR)/generic
DLLFULLNAME = $(OUTDIR)/$(DLLNAME)
MAP = $(OUTDIR)/$(NAME).map

EXEEXT		= .exe
OBJEXT		= o
LIBEXT    = a
STLIB_LD	= ${AR} cr
SHLIB_LD	= ${CC} -shared
SHLIB_LD_LIBS	= ${LIBS}
SHLIB_CFLAGS	=
SHLIB_SUFFIX	= .dll
LIBS          = -lrpcrt4

# warning flags
CFLAGS_WARNING = -Wall -Wwrite-strings -Wsign-compare -Wdeclaration-after-statement
# The default switches for optimization or debugging
CFLAGS_DEBUG    = -g
CFLAGS_OPTIMIZE	= -O2 -fomit-frame-pointer

CFLAGS          = -m32 -fno-rtti -fno-exceptions -DUNICODE -std=gnu++0x $(CFLAGS_OPTIMIZE)

ifdef DEBUG
CFLAGS_DBG = -g
LDFLAGS_DBG = -Wl,-Map=$(MAP)
else
LDFLAGS_DBG = -Wl,--exclude-all-symbols
endif

LINK_OBJS = $(OBJDIR)/$(NAME).$(OBJEXT) \
            $(TCLSTUB) \
            $(OBJDIR)/$(NAME).res.$(OBJEXT)

VER_MAJOR = $(shell grep "define VER_MAJOR" version.hpp | grep -oE "[[:digit:]]+")
VER_MINOR = $(shell grep "define VER_MINOR" version.hpp | grep -oE "[[:digit:]]+")
VER_REVIS = $(shell grep "define VER_REVIS" version.hpp | grep -oE "[[:digit:]]+")
VER_BUILD = $(shell grep "define VER_BUILD" version.hpp | grep -oE "[[:digit:]]+")
VER_FULL  = $(VER_MAJOR).$(VER_MINOR).$(VER_REVIS).$(VER_BUILD)

VER_BUILD_INCR = $(shell expr 1 + `grep "define VER_BUILD" version.hpp | grep -oE "[[:digit:]]+"`)

SOURCES = FarTcl.cpp \
          farAPI_Basic.cpp \
          farAPI_Editor.cpp \
          farAPI_PluginManager.cpp \
          farAPI_Tcl.cpp \
          FarTclMessages.hpp \
          guid.hpp \
          version.hpp

SOURCES_FARLIB = farlib/farflags.tcl \
                 farlib/farlib.tcl \
                 farlib/pkgIndex.tcl

SOURCES_ADDON = addon/FarTcl.lua \
                addon/test.tcl

all: build

$(OBJDIR)/$(NAME).$(OBJEXT): $(SOURCES) include/plugin.hpp $(TCLDIR)/generic/tcl.h
	sed -i 's/define VER_BUILD .*/define VER_BUILD $(VER_BUILD_INCR)/' version.hpp
	CPATH=$(INCLUDE) $(CPP) $(CFLAGS) $(CFLAGS_DBG) -c -o $@ $<

FarTclMessages.hpp: FarTclMessages.feed
	@echo generating language files
	@tools/lng.generator.exe -nc -ol "$(OUTDIR)" FarTclMessages.feed

$(DLLFULLNAME): $(LINK_OBJS)
	@rm -f $(DLLFULLNAME)
	${SHLIB_LD} $(LDFLAGS) $(LDFLAGS_DBG) -o $@ -pipe -static-libgcc -municode -Wl,--kill-at $(LINK_OBJS) $(SHLIB_LD_LIBS)

$(OBJDIR)/$(NAME).res.$(OBJEXT): $(NAME).rc version.hpp
	$(RC) -o $@ --include "include" $<

$(OBJDIR): $(OUTDIR)
	mkdir -p "$(OBJDIR)"

$(OUTDIR):
	mkdir -p "$(OUTDIR)"

$(OUTDIR)/lib: $(OUTDIR)
	mkdir -p "$(OUTDIR)/lib"

$(OUTDIR)/tcl$(TCLVER).dll: $(OUTDIR) $(TCLDIR)/win/tcl$(TCLVER).dll
	cp "$(TCLDIR)/win/tcl$(TCLVER).dll" "$(OUTDIR)"
	find "$(TCLDIR)/win" -name "zlib*.dll" -exec cp {} "$(OUTDIR)" \;

$(OUTDIR)/lib/tcl: $(OUTDIR)/lib
	mkdir -p "$@"
	cd "$(TCLDIR)/library" && cp -r * "$@"
	find "$(TCLDIR)/win" -name "tcldde*.dll" -exec cp {} "$@/dde" \;
	find "$(TCLDIR)/win" -name "tclreg*.dll" -exec cp {} "$@/reg" \;

$(OUTDIR)/lib/procarg: $(OUTDIR)/lib src/procarg
	mkdir -p "$@"
	cd "src/procarg" && cp -r * "$@"

$(OUTDIR)/lib/farlib: $(OUTDIR)/lib $(SOURCES_FARLIB)
	mkdir -p "$@"
	cp $(SOURCES_FARLIB) "$@"

farlib/farflags.tcl: include/plugin.hpp
	$(TCLSH) farlib/_genflags.tcl include/plugin.hpp >farlib/farflags.tcl

$(OUTDIR)/addon: $(OUTDIR)/lib $(SOURCES_ADDON)
	mkdir -p "$@"
	cp $(SOURCES_ADDON) "$@"

$(TCLSH) $(TCLSTUB) $(TCLDIR)/win/tcl$(TCLVER).dll:
# convert line ending in configure script: crlf to lf
	cat $(TCLDIR)/win/configure | tr -d '\r' > $(TCLDIR)/win/configure.fixed
	rm -f $(TCLDIR)/win/configure
	mv $(TCLDIR)/win/configure.fixed $(TCLDIR)/win/configure
	chmod 755 $(TCLDIR)/win/configure
	cd $(TCLDIR)/win && $(TCLCONFFLAGS) ./configure --enable-threads
	make -C $(TCLDIR)/win all

build: $(OUTDIR) $(OBJDIR) $(DLLFULLNAME) $(OUTDIR)/tcl$(TCLVER).dll $(OUTDIR)/lib/tcl $(OUTDIR)/lib/farlib $(OUTDIR)/lib/procarg $(OUTDIR)/addon
	@echo Build DONE.

clean:
	rm -f FarTclMessages.hpp
	rm -f farlib/farflags.tcl
	rm -rf out.*
	make -C $(TCLDIR)/win clean || echo "Already cleaned"

distclean: clean
	make -C $(TCLDIR)/win distclean || echo "Already cleaned"

dist: build
	cd "$(OUTDIR)" && rm -rf obj && ../tools/zip.exe -r ../../fartcl-$(VER_FULL)_$(_ODB_).zip *

srcdist: distclean
	./tools/zip.exe -r ../fartcl-$(VER_FULL)_src.zip *

install: build
	rm -rf "$(INSTALL_PREFIX)/addon"
	rm -rf "$(INSTALL_PREFIX)/lib"
	rm -rf "$(INSTALL_PREFIX)/*.lng"
	mv "$(INSTALL_PREFIX)/FarTcl.dll" "$(INSTALL_PREFIX)/FarTcl.dll.$$RANDOM.d__" || echo "Not found"
	find "$(INSTALL_PREFIX)" -name "tcl*.dll" -exec mv {} "$(INSTALL_PREFIX)/tcl.dll.$$RANDOM.d__" \;
	find "$(INSTALL_PREFIX)" -name "zlib*.dll" -exec mv {} "$(INSTALL_PREFIX)/zlib.dll.$$RANDOM.d__" \;
	cd "$(OUTDIR)" && cp -r * "$(INSTALL_PREFIX)"

.PHONY: all build srcdist clean distclean dist install
