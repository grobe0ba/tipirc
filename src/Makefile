# Copyright (c) 2021 B. Atticus Grobe (grobe0ba@gmail.com)
#
# Permission to use, copy, modify, and distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

ROOT !!= readlink -f ..
IOBJ= ${ROOT}/iobj

.ifnmake clean
.BEGIN:
	make -C "${ROOT}/third_party" install
	${IOBJ}/makeheaders *.cc
	${IOBJ}/makeheaders -H *.cc > tipirc_public.h >/dev/null 2>&1
.endif

LIB= tipirc
SRCS= rpl.cc cmds.cc tipirc.cc
CLEANFILES= rpl.hh cmds.hh tipirc.hh tipirc_public.h
CXXOPTS= -std=c++17 \
		 -I/usr/local/include \
		 -I"${IOBJ}/include"

LDADD= -L/usr/local/lib \
	   -L"${IOBJ}/lib" \
	   -lgflags -lglog

HDRS= tipirc_public.h

.include "${ROOT}/Makefile.inc"

.ifmake install
DESTDIR !!= readlink -f ${ROOT}/iobj
.endif

.include <bsd.lib.mk>
