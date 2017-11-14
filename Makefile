##
## Makefile for Flex Socket

PLATFORM=dist
SRCDIR=.

VERSION=1.0
SHLIB_VERSION_NUMBER=1.0
SHLIB_MAJOR=1
SHLIB_MONOR=0

LIBS=libflex.a
INSTALLTOP=/usr/local
LIBDIR=lib

PROGRAMS=test_app/publisher test_app/subscriber
LIBRPATH=$(INSTALLTOP)/$(LIBDIR)

CC= cc
CFLAGS=-O
LDFLAGS=

ARFLAGS=
AR=ar $(ARFLAGS) r
RANLIB=ranlib

DESTDIR=
RM= rm -f
RMDIR= rmdir
BASENAME= flex
NAME= $(BASENAME)-$(VERSION)

OBJ=src/behavior.o src/flex_id.o

all: libflex.a

install:

uninstall:

libclean:
	@set -e; for s in $(SHLIB_INFO); do\
		s1=`echo "$$s" | cut -f1 -d";"`; \
		s2=`echo "$$s" | cut -f2 -d";"`; \
		echo $(RM) $$s1; \
		$(RM) $$s1; \
		if [ "$$s1" != "$$s2" ]; then \
			echo $(RM) $$s2; \
			$(RM) $$s2; \
		fi; \
	done
	$(RM) $(LIBS)
	$(RM) *.map

clean: libclean
	$(RM) $(OBJ)
	-$(RM) `find . -name '*.o' -a \! -path "./.git*"`

libflex.pc:
	@ ( echo 'prefix=$(INSTALLTOP)'; \
		 	echo 'exec_prefix=$${prefix}'; \
			echo 'libdir=$${exec_prefix}/$(LIBDIR)'; \
			echo 'includedif=$${prefix}/include'; \
			echo ''; \
			echo 'Name: Flex-libflex'; \
			echo 'Description: Flex ID based networking library'; \
			echo 'Version: '$(VERSION); \
			echo 'Libs: -L$${libdir} -lflex'; \
			echo 'Cflags: -I$${includedir}' ) > libflex.pc

flex.pc:
	@ ( echo 'prefix=$(INSTALLTOP)'; \
			echo 'exec_prefix=$${prefix}'; \
			echo 'libdir=$${exec_prefix}/$(LIBDIR)'; \
			echo 'includedir=$${prefix}/include'; \
			echo ''; \
			echo 'Name: Flex'; \
			echo 'Description: Flex ID based networking library'; \
			echo 'Version: '$(VERSION); \
			echo 'Requires: libflex' ) > flex.pc

libflex.a: src/behavior.o src/flex_id.o
	$(AR) $@ $?
	$(RANLIB) $@

src/behavior.o: src/behavior.c
	$(CC) -Iinclude -o $@ -c src/behavior.c

src/flex_id.o: src/flex_id.c
	$(CC) -Iinclude -o $@ -c src/flex_id.c
