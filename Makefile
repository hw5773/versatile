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
INSTALL_LIBS=libflex.a
LIBDIR=lib

APPDIR=apps/flex
PROGRAMS=apps/flex/publisher apps/flex/subscriber
TCPAPPDIR=apps/ipv4
TCPPROGRAMS=apps/ipv4/server apps/ipv4/client
LIBRPATH=$(INSTALLTOP)/$(LIBDIR)

CC= cc
CFLAGS=-O -DDEBUG -I/usr/local/include
LDFLAGS= -L/usr/local/lib -lssl -lcrypto -ldl

ARFLAGS=
AR=ar $(ARFLAGS) r
RANLIB=ranlib

SRCDIR=`pwd`
DESTDIR=
RM= rm -f
RMDIR= rmdir
BASENAME= flex
NAME= $(BASENAME)-$(VERSION)

SOCKDIR=socket
SOCK_MODULE=flex.ko

PCS=libflex.pc flex.pc

OBJ=src/behavior.o src/flex_id.o src/error.o src/test_func.o src/request.o src/supplement.o src/repo.o src/hash_table.o src/hash_function.o

all: libflex.a libflex.pc flex.pc

install: all install_lib install_socket install_apps

install_lib:
	@[ -n "$(INSTALLTOP)" ] || (echo INSTALLTOP should not be empty; exit 1)
	@echo "*** Installing development files"
	@$(SRCDIR)/util/mkdir-p.pl $(INSTALLTOP)/include/flex
	@ :
	@set -e; for i in $(SRCDIR)/include/flex/*.h; do \
		fn=`basename $$i`; \
		echo "install $$i -> $(INSTALLTOP)/include/flex/$$fn"; \
		cp $$i $(INSTALLTOP)/include/flex/$$fn; \
		chmod 644 $(INSTALLTOP)/include/flex/$$fn; \
	done
	@$(SRCDIR)/util/mkdir-p.pl $(INSTALLTOP)/$(LIBDIR)
	@set -e; for l in $(INSTALL_LIBS); do \
		fn=`basename $$l`; \
		echo "install $$l -> $(INSTALLTOP)/$(LIBDIR)/$$fn"; \
		cp $$l $(INSTALLTOP)/$(LIBDIR)/$$fn.new; \
		$(RANLIB) $(INSTALLTOP)/$(LIBDIR)/$$fn.new; \
		chmod 644 $(INSTALLTOP)/$(LIBDIR)/$$fn.new; \
		mv -f $(INSTALLTOP)/$(LIBDIR)/$$fn.new \
					$(INSTALLTOP)/$(LIBDIR)/$$fn; \
	done
	@ :
	@$(SRCDIR)/util/mkdir-p.pl $(INSTALLTOP)/$(LIBDIR)/pkgconfig
	@echo "install libflex.pc -> $(INSTALLTOP)/$(LIBDIR)/pkgconfig/libflex.pc"
	@cp libflex.pc $(INSTALLTOP)/$(LIBDIR)/pkgconfig
	@chmod 644 $(INSTALLTOP)/$(LIBDIR)/pkgconfig/libflex.pc
	@echo "install flex.pc -> $(INSTALLTOP)/$(LIBDIR)/pkgconfig/flex.pc"
	@cp flex.pc $(INSTALLTOP)/$(LIBDIR)/pkgconfig
	@chmod 644 $(INSTALLTOP)/$(LIBDIR)/pkgconfig/flex.pc

install_socket:
	@echo "Make Flex Network Socket"
	(cd $(SOCKDIR); make; cd $(SRCDIR))
	@echo "Insert Flex Network Socket"
	@insmod $(SRCDIR)/$(SOCKDIR)/$(SOCK_MODULE)

install_apps:
	@echo "Make Flex Test Application"
	(cd $(SRCDIR)/$(APPDIR); make; cp publisher /usr/local/bin/flex_publisher; cp subscriber /usr/local/bin/flex_subscriber; cp subscriber10 /usr/local/bin/flex_subscriber10; cp subscriber15 /usr/local/bin/flex_subscriber15; cd $(SRCDIR))
	(cd $(SRCDIR)/$(TCPAPPDIR); make; cp server /usr/local/bin/tcpip_publisher; cp client /usr/local/bin/tcpip_subscriber; cd $(SRCDIR))

uninstall: uninstall_lib uninstall_socket uninstall_apps

uninstall_lib:
	@echo "*** Uninstalling development files"
	@ :
	@set -e; for i in $(SRCDIR)/include/flex/*.h; do \
		fn=`basename $$i`; \
		echo "$(RM) $(INSTALLTOP)/include/flex/$$fn"; \
		$(RM) $(INSTALLTOP)/include/flex/$$fn; \
	done
	-$(RMDIR) $(INSTALLTOP)/include/flex
	@set -e; for l in $(INSTALL_LIBS); do \
		fn=`basename $$l`; \
		echo "$(RM) $(INSTALLTOP)/$(LIBDIR)/$$fn"; \
		$(RM) $(INSTALLTOP)/$(LIBDIR)/$$fn; \
	done
	@ :
	$(RM) $(INSTALLTOP)/$(LIBDIR)/pkgconfig/libflex.pc
	$(RM) $(INSTALLTOP)/$(LIBDIR)/pkgconfig/flex.pc

uninstall_socket:
	@echo "Remove Flex Network Socket"
	@rmmod $(SRCDIR)/$(SOCKDIR)/$(SOCK_MODULE)
	(cd $(SOCKDIR);make clean; cd $(SRCDIR))

uninstall_apps:
	@echo "Remove Flex Test Applications"
	@rm /usr/local/bin/flex_publisher /usr/local/bin/flex_subscriber /usr/local/bin/flex_subscriber10 /usr/local/bin/flex_subscriber15 /usr/local/bin/tcpip_publisher /usr/local/bin/tcpip_subscriber

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
	$(RM) $(OBJ) $(PCS) $(PROGRAMS) $(TCPPROGRAMS)
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

libflex.a: $(OBJ)
	$(AR) $@ $?
	$(RANLIB) $@

src/behavior.o: src/behavior.c
	$(CC) $(CFLAGS) -Iinclude -o $@ -c $<

src/flex_id.o: src/flex_id.c
	$(CC) $(CFLAGS) -Iinclude -o $@ -c $<

src/error.o: src/error.c
	$(CC) $(CFLAGS) -Iinclude -o $@ -c $<

src/request.o: src/request.c
	$(CC) $(CFLAGS) -Iinclude -o $@ -c $<

src/test_func.o: src/test_func.c
	$(CC) $(CFLAGS) -Iinclude -o $@ -c $<

src/supplement.o: src/supplement.c
	$(CC) $(CFLAGS) -Iinclude -o $@ -c $<

src/repo.o: src/repo.c
	$(CC) $(CFLAGS) -Iinclude -o $@ -c $<

src/hash_table.o: src/hash_table.c
	$(CC) $(CFLAGS) -Iinclude -o $@ -c $<

src/hash_function.o: src/hash_function.c
	$(CC) $(CFLAGS) -Iinclude -o $@ -c $<
