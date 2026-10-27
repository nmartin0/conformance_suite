/*
 * iv.h - helpers for the iconv tests: find a pair of code set names that
 * the system accepts, run one iconv() call and report all it changed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * The standard names no code set, so every test probes a short list of
 * familiar names and is UNRESOLVED when none of them opens. The lists
 * hold only code sets in which the portable characters keep their usual
 * single byte values (XBD 6.1, 6.2).
 */
#ifndef IV_H
#define IV_H

#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <iconv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define IV_BAD ((iconv_t)-1)

static const char *const iv_all[] = { "UTF-8", "US-ASCII", "ASCII", "ISO-8859-1", "ISO8859-1", NULL };
static const char *const iv_utf8[] = { "UTF-8", NULL };
static const char *const iv_ascii[] = { "US-ASCII", "ASCII", NULL };
static const char *const iv_latin1[] = { "ISO-8859-1", "ISO8859-1", NULL };
static const char *const iv_target[] = { "US-ASCII", "ASCII", "ISO-8859-1", "ISO8859-1", NULL };

/*
 * Open a conversion from one of froms to one of tos, appending suffix (or
 * nothing) to the target name. Returns IV_BAD when no combination opens.
 */
static inline iconv_t iv_open(const char *const *tos, const char *const *froms,
    const char *suffix, const char **tp, const char **fp)
{
	char to[80];
	size_t i, j;
	iconv_t cd;

	for (i = 0; tos[i] != NULL; i++) {
		for (j = 0; froms[j] != NULL; j++) {
			if (strcmp(tos[i], froms[j]) == 0)
				continue;
			(void)snprintf(to, sizeof to, "%s%s", tos[i], suffix != NULL ? suffix : "");
			cd = iconv_open(to, froms[j]);
			if (cd != IV_BAD) {
				if (tp != NULL)
					*tp = tos[i];
				if (fp != NULL)
					*fp = froms[j];
				return cd;
			}
		}
	}
	return IV_BAD;
}

/* UNRESOLVED unless a pair opened. */
static inline void iv_need(iconv_t cd)
{
	if (cd == IV_BAD)
		pc_unresolved("none of the probed code set names could be opened (names are implementation-defined)");
}

struct iv_res {
	size_t ret;
	int err;
	size_t inleft, outleft;
	char *inp, *outp;
};

/* One iconv() call on in[0..n) into out[0..outsz). */
static inline struct iv_res iv_call(iconv_t cd, char *in, size_t n, char *out, size_t outsz)
{
	struct iv_res r;
	char *ip = in;
	char *op = out;
	size_t il = n;
	size_t ol = outsz;

	errno = 0;
	r.ret = iconv(cd, &ip, &il, &op, &ol);
	r.err = errno;
	r.inleft = il;
	r.outleft = ol;
	r.inp = ip;
	r.outp = op;
	return r;
}

#define IV_FDS 1024

/* Which descriptors below IV_FDS are open. */
static inline void iv_fdmap(unsigned char *m)
{
	int fd;

	for (fd = 0; fd < IV_FDS; fd++)
		m[fd] = fcntl(fd, F_GETFD) != -1;
}

#endif
