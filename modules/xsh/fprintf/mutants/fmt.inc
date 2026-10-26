/*
 * fmt.inc - a small formatter shared by the fprintf() fault and variant
 * libraries (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * It walks a format, fetches each argument with its own type, and has the C
 * library's own snprintf() convert one conversion specification at a time,
 * so that a planted fault or a variant can change one conversion and leave
 * every other one exactly as the system does it. Numbered ("%n$")
 * arguments are all fetched first, in order, by the types the format gives
 * them. The including file defines the fe_hook_* functions.
 */
#include <dlfcn.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <wchar.h>

enum { FE_NONE, FE_INT, FE_UINT, FE_LONG, FE_ULONG, FE_LLONG, FE_ULLONG,
	FE_IMAX, FE_UMAX, FE_SSIZE, FE_SIZE, FE_PDIFF, FE_DBL, FE_LDBL,
	FE_WINT, FE_PTR };

union fe_val {
	intmax_t i;
	uintmax_t u;
	double d;
	long double ld;
	wint_t wc;
	void *p;
};

struct fe_spec {
	int pos;		/* n of "%n$", 0 when unnumbered */
	int minus, plus, space, alt, zero, apos;
	int width, wstar, wpos;	/* width -1: none */
	int prec, pstar, ppos;	/* prec -1: none */
	char len[3];
	char conv;
	int type;
	int mark;		/* free for the hooks */
	int keep;		/* free for the hooks */
};

struct fe_out {
	char *b;
	size_t n, cap;
	int err;		/* errno value of the first error, 0 if none */
};

#define FE_MAXARG 64

struct fe_args {
	va_list *ap;
	int positional;
	int next;
	int types[FE_MAXARG + 1];
	union fe_val tab[FE_MAXARG + 1];
};

/* Hooks, defined by the including file. */
static int fe_hook_text(struct fe_out *o, const char *s, size_t n, int last);
static int fe_hook_percent(struct fe_out *o, struct fe_args *a);
static void fe_hook_stars(struct fe_spec *sp, int *w, int *p);
static void fe_hook_spec(struct fe_spec *sp, union fe_val *v);
static int fe_hook_piece(struct fe_out *o, const struct fe_spec *sp, union fe_val v,
    const char *piece, size_t n);
static int fe_hook_fail(struct fe_out *o, const struct fe_spec *sp, union fe_val v);
static void fe_hook_n(struct fe_out *o, struct fe_spec *sp, size_t *count);
static int fe_hook_posmode(void);	/* 1: numbers ignored; 2: "*m$" ignored */
static void fe_hook_type(struct fe_spec *sp);	/* may change how the argument is fetched */

typedef int fe_snprintf_fn(char *, size_t, const char *, ...);

static fe_snprintf_fn *fe_real(void)
{
	static fe_snprintf_fn *p;

	if (p == NULL)
		p = (fe_snprintf_fn *)dlsym(RTLD_NEXT, "snprintf");
	return p;
}

static void fe_fail(struct fe_out *o, int e)
{
	if (o->err == 0)
		o->err = e;
}

static int fe_room(struct fe_out *o, size_t n)
{
	char *nb;
	size_t c;

	if (o->err)
		return -1;
	if (n > (size_t)INT_MAX - o->n) {
		fe_fail(o, EOVERFLOW);
		return -1;
	}
	if (o->n + n + 1 <= o->cap)
		return 0;
	c = o->cap ? o->cap : 256;
	while (c < o->n + n + 1)
		c *= 2;
	nb = realloc(o->b, c);
	if (nb == NULL) {
		fe_fail(o, ENOMEM);
		return -1;
	}
	o->b = nb;
	o->cap = c;
	return 0;
}

static void fe_put(struct fe_out *o, const char *s, size_t n)
{
	if (fe_room(o, n) != 0)
		return;
	memcpy(o->b + o->n, s, n);
	o->n += n;
	o->b[o->n] = '\0';
}

static int fe_isint(char c)
{
	return c != '\0' && strchr("diouxX", c) != NULL;
}

static int fe_isfloat(char c)
{
	return c != '\0' && strchr("fFeEgGaA", c) != NULL;
}

static int fe_type(const struct fe_spec *sp)
{
	const char *l = sp->len;
	int sig = sp->conv == 'd' || sp->conv == 'i';

	if (fe_isint(sp->conv)) {
		if (strcmp(l, "l") == 0)
			return sig ? FE_LONG : FE_ULONG;
		if (strcmp(l, "ll") == 0)
			return sig ? FE_LLONG : FE_ULLONG;
		if (strcmp(l, "j") == 0)
			return sig ? FE_IMAX : FE_UMAX;
		if (strcmp(l, "z") == 0)
			return sig ? FE_SSIZE : FE_SIZE;
		if (strcmp(l, "t") == 0)
			return FE_PDIFF;
		return sig ? FE_INT : FE_UINT;
	}
	if (fe_isfloat(sp->conv))
		return strcmp(l, "L") == 0 ? FE_LDBL : FE_DBL;
	if (sp->conv == 'c')
		return strcmp(l, "l") == 0 ? FE_WINT : FE_INT;
	if (sp->conv == 'C')
		return FE_WINT;
	if (sp->conv == 's' || sp->conv == 'S' || sp->conv == 'p' || sp->conv == 'n')
		return FE_PTR;
	return FE_NONE;
}

static union fe_val fe_fetch(va_list *ap, int type)
{
	union fe_val v;

	memset(&v, 0, sizeof v);
	switch (type) {
	case FE_INT: v.i = va_arg(*ap, int); break;
	case FE_UINT: v.u = va_arg(*ap, unsigned); break;
	case FE_LONG: v.i = va_arg(*ap, long); break;
	case FE_ULONG: v.u = va_arg(*ap, unsigned long); break;
	case FE_LLONG: v.i = va_arg(*ap, long long); break;
	case FE_ULLONG: v.u = va_arg(*ap, unsigned long long); break;
	case FE_IMAX: v.i = va_arg(*ap, intmax_t); break;
	case FE_UMAX: v.u = va_arg(*ap, uintmax_t); break;
	case FE_SSIZE: v.i = va_arg(*ap, ssize_t); break;
	case FE_SIZE: v.u = va_arg(*ap, size_t); break;
	case FE_PDIFF: v.i = va_arg(*ap, ptrdiff_t); break;
	case FE_DBL: v.d = va_arg(*ap, double); break;
	case FE_LDBL: v.ld = va_arg(*ap, long double); break;
	case FE_WINT: v.wc = va_arg(*ap, wint_t); break;
	case FE_PTR: v.p = va_arg(*ap, void *); break;
	default: break;
	}
	return v;
}

/* The library's own conversion of one specification (without "n$"). */
static int fe_conv(char *b, size_t n, const char *sub, int type, union fe_val v)
{
	fe_snprintf_fn *f = fe_real();

	switch (type) {
	case FE_INT: return f(b, n, sub, (int)v.i);
	case FE_UINT: return f(b, n, sub, (unsigned)v.u);
	case FE_LONG: return f(b, n, sub, (long)v.i);
	case FE_ULONG: return f(b, n, sub, (unsigned long)v.u);
	case FE_LLONG: return f(b, n, sub, (long long)v.i);
	case FE_ULLONG: return f(b, n, sub, (unsigned long long)v.u);
	case FE_IMAX: return f(b, n, sub, v.i);
	case FE_UMAX: return f(b, n, sub, v.u);
	case FE_SSIZE: return f(b, n, sub, (ssize_t)v.i);
	case FE_SIZE: return f(b, n, sub, (size_t)v.u);
	case FE_PDIFF: return f(b, n, sub, (ptrdiff_t)v.i);
	case FE_DBL: return f(b, n, sub, v.d);
	case FE_LDBL: return f(b, n, sub, v.ld);
	case FE_WINT: return f(b, n, sub, v.wc);
	case FE_PTR: return f(b, n, sub, v.p);
	default: return f(b, n, sub);
	}
}

static int fe_num(const char **pp)
{
	long n = 0;

	while (**pp >= '0' && **pp <= '9') {
		if (n < INT_MAX / 10)
			n = n * 10 + (**pp - '0');
		(*pp)++;
	}
	return (int)n;
}

/* A digit string followed by '$' at *pp: its value, else 0 (pp unchanged). */
static int fe_dollar(const char **pp)
{
	const char *q = *pp;
	int n;

	if (*q < '1' || *q > '9')
		return 0;
	n = fe_num(&q);
	if (*q != '$')
		return 0;
	*pp = q + 1;
	return n;
}

/* Parses the specification after '%' at *pp; 0 on success. */
static int fe_parse(const char **pp, struct fe_spec *sp)
{
	const char *p = *pp;

	memset(sp, 0, sizeof *sp);
	sp->width = sp->prec = -1;
	sp->pos = fe_dollar(&p);
	for (;; p++) {
		if (*p == '-') sp->minus = 1;
		else if (*p == '+') sp->plus = 1;
		else if (*p == ' ') sp->space = 1;
		else if (*p == '#') sp->alt = 1;
		else if (*p == '0') sp->zero = 1;
		else if (*p == '\'') sp->apos = 1;
		else break;
	}
	if (*p == '*') {
		p++;
		sp->wstar = 1;
		sp->wpos = fe_dollar(&p);
	} else if (*p >= '1' && *p <= '9') {
		sp->width = fe_num(&p);
	}
	if (*p == '.') {
		p++;
		if (*p == '*') {
			p++;
			sp->pstar = 1;
			sp->ppos = fe_dollar(&p);
		} else {
			sp->prec = fe_num(&p);
		}
	}
	if ((p[0] == 'h' && p[1] == 'h') || (p[0] == 'l' && p[1] == 'l')) {
		sp->len[0] = p[0];
		sp->len[1] = p[1];
		p += 2;
	} else if (*p != '\0' && strchr("hljztL", *p) != NULL) {
		sp->len[0] = *p++;
	}
	if (*p == '\0' || strchr("diouxXfFeEgGaAcspnCS%", *p) == NULL)
		return -1;
	sp->conv = *p++;
	sp->type = fe_type(sp);
	fe_hook_type(sp);
	*pp = p;
	return 0;
}

static int fe_valid_pos(int n)
{
	return n >= 1 && n <= FE_MAXARG;
}

/* Fetches every numbered argument, in order, by the types the format gives. */
static void fe_prefetch(const char *fmt, struct fe_args *a)
{
	const char *p = fmt;
	struct fe_spec sp;
	int max = 0, i;

	memset(a->types, 0, sizeof a->types);
	while ((p = strchr(p, '%')) != NULL) {
		p++;
		if (fe_parse(&p, &sp) != 0 || sp.conv == '%')
			continue;
		if (sp.pos == 0)
			continue;
		a->positional = 1;
		if (fe_valid_pos(sp.pos)) {
			a->types[sp.pos] = sp.type;
			if (sp.pos > max)
				max = sp.pos;
		}
		if (fe_valid_pos(sp.wpos)) {
			a->types[sp.wpos] = FE_INT;
			if (sp.wpos > max)
				max = sp.wpos;
		}
		if (fe_valid_pos(sp.ppos)) {
			a->types[sp.ppos] = FE_INT;
			if (sp.ppos > max)
				max = sp.ppos;
		}
	}
	for (i = 1; i <= max; i++)
		a->tab[i] = fe_fetch(a->ap, a->types[i] ? a->types[i] : FE_INT);
}

/* The argument for position n (0: the next one). */
static union fe_val fe_arg(struct fe_args *a, int n, int type)
{
	union fe_val z;

	if (!a->positional)
		return fe_fetch(a->ap, type);
	if (n == 0 || (fe_hook_posmode() & 1))
		n = ++a->next;
	if (fe_valid_pos(n))
		return a->tab[n];
	memset(&z, 0, sizeof z);
	return z;
}

static void fe_store_n(const struct fe_spec *sp, void *p, size_t count)
{
	const char *l = sp->len;

	if (p == NULL)
		return;
	if (strcmp(l, "hh") == 0) *(signed char *)p = (signed char)count;
	else if (strcmp(l, "h") == 0) *(short *)p = (short)count;
	else if (strcmp(l, "l") == 0) *(long *)p = (long)count;
	else if (strcmp(l, "ll") == 0) *(long long *)p = (long long)count;
	else if (strcmp(l, "j") == 0) *(intmax_t *)p = (intmax_t)count;
	else if (strcmp(l, "z") == 0) *(ssize_t *)p = (ssize_t)count;
	else if (strcmp(l, "t") == 0) *(ptrdiff_t *)p = (ptrdiff_t)count;
	else *(int *)p = (int)count;
}

/* The unnumbered specification for the library: flags, width, precision. */
static void fe_sub(char *sub, size_t size, const struct fe_spec *sp)
{
	char *q = sub;

	*q++ = '%';
	if (sp->minus) *q++ = '-';
	if (sp->plus) *q++ = '+';
	if (sp->space) *q++ = ' ';
	if (sp->alt) *q++ = '#';
	if (sp->zero) *q++ = '0';
	if (sp->apos) *q++ = '\'';
	*q = '\0';
	if (sp->width >= 0)
		q += fe_real()(q, size - (size_t)(q - sub), "%d", sp->width);
	if (sp->prec >= 0)
		q += fe_real()(q, size - (size_t)(q - sub), ".%d", sp->prec);
	(void)fe_real()(q, size - (size_t)(q - sub), "%s%c", sp->len, sp->conv);
}

/* Converts one parsed specification into o. */
static void fe_one(struct fe_out *o, struct fe_spec *sp, struct fe_args *a)
{
	char sub[64], small[512], *buf;
	union fe_val v;
	int w = 0, p = 0, n;

	if (sp->wstar)
		w = (int)fe_arg(a, sp->wpos, FE_INT).i;
	if (sp->pstar)
		p = (int)fe_arg(a, sp->ppos, FE_INT).i;
	if (a->positional && (fe_hook_posmode() & 2)) {
		if (sp->wpos)
			sp->wstar = 0;
		if (sp->ppos)
			sp->pstar = 0;
	}
	fe_hook_stars(sp, &w, &p);
	if (sp->wstar) {
		if (w < 0) {
			sp->minus = 1;
			w = w == INT_MIN ? INT_MAX : -w;
		}
		sp->width = w;
	}
	if (sp->pstar)
		sp->prec = p < 0 ? -1 : p;
	v = fe_arg(a, sp->pos, sp->type);
	if (sp->conv == 'n') {
		size_t count = o->n;

		fe_hook_n(o, sp, &count);
		fe_store_n(sp, v.p, count);
		return;
	}
	fe_hook_spec(sp, &v);
	fe_sub(sub, sizeof sub, sp);
	n = fe_conv(NULL, 0, sub, sp->type, v);
	if (n < 0) {
		if (!fe_hook_fail(o, sp, v))
			fe_fail(o, errno ? errno : EINVAL);
		return;
	}
	if ((size_t)n > (size_t)INT_MAX - o->n) {
		fe_fail(o, EOVERFLOW);
		return;
	}
	buf = (size_t)n < sizeof small ? small : malloc((size_t)n + 1);
	if (buf == NULL) {
		fe_fail(o, ENOMEM);
		return;
	}
	(void)fe_conv(buf, (size_t)n + 1, sub, sp->type, v);
	if (!fe_hook_piece(o, sp, v, buf, (size_t)n))
		fe_put(o, buf, (size_t)n);
	if (buf != small)
		free(buf);
}

/*
 * Formats fmt with the arguments in *ap into a new string (*res, *len); 0,
 * or -1 with errno set.
 */
static int fe_run(const char *fmt, va_list *ap, char **res, size_t *len)
{
	struct fe_out o;
	struct fe_args a;
	struct fe_spec sp;
	const char *p = fmt, *q;
	int e = errno;

	memset(&o, 0, sizeof o);
	memset(&a, 0, sizeof a);
	a.ap = ap;
	if (fe_room(&o, 0) != 0) {
		errno = o.err;
		return -1;
	}
	o.b[0] = '\0';
	fe_prefetch(fmt, &a);
	while (*p != '\0' && !o.err) {
		q = strchr(p, '%');
		if (q == NULL)
			q = p + strlen(p);
		if (q > p) {
			if (!fe_hook_text(&o, p, (size_t)(q - p), *q == '\0'))
				fe_put(&o, p, (size_t)(q - p));
			p = q;
			continue;
		}
		p++;
		q = p;
		if (fe_parse(&p, &sp) != 0) {
			fe_put(&o, "%", 1);	/* undefined: copied as it is */
			continue;
		}
		if (sp.conv == '%') {
			if (!fe_hook_percent(&o, &a))
				fe_put(&o, "%", 1);
			continue;
		}
		(void)q;
		fe_one(&o, &sp, &a);
	}
	if (o.err) {
		free(o.b);
		errno = o.err;
		return -1;
	}
	*res = o.b;
	*len = o.n;
	errno = e;
	return 0;
}
