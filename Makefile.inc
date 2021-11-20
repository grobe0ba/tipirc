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

BINOWN !!= id -nu
BINGRP !!= id -ng
LIBOWN= ${BINOWN}
LIBGRP= ${BINGRP}
LIBDIR= /lib
DOCOWN= ${BINOWN}
DOCGRP= ${BINGRP}
LOCALEOWN= ${BINOWN}
LOCALEGRP= ${BINGRP}
MANOWN= ${BINOWN}
MANGRP= ${BINGRP}
SHAREOWN= ${BINOWN}
SHAREGRP= ${BINGRP}

includes:
	for i in ${HDRS}; do \
        "${ROOT}/hdrinst.sh" "$${i}" "${DESTDIR}/include/$${i}"; \
    done

afterinstall: realinstall includes
