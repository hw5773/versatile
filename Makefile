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
GENERATED_MANDATORY=include/flex.h include/proto_flex.h include/flexid.h

CC= cc
CFLAGS=-O
LDFLAGS=

DESTDIR=
RM= rm -f
RMDIR= rmdir
BASENAME= flex
NAME= $(BASENAME)-$(VERSION)

all: build_generated

build_generated: $(GENERATED_MANDATORY)

install:

clean:

libflex.a: src/
	$(AR) $@ $?
