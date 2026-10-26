/*
 * fscanf.c - planted faults in fscanf(), scanf() and sscanf(), for
 * mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "fscanf.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process or a parent).
 *
 * With no fault active every call goes straight to the C library. With a
 * fault active, a small driver runs the format one directive at a time:
 * white space and ordinary characters are matched here, each conversion is
 * handed to the C library's own vsscanf()/vfscanf() with "%n" appended,
 * and the faults bend one step of that. glibc compiles calls in strict
 * modes to __isoc99_* names, so those are interposed as well.
 */
#define _GNU_SOURCE 1
#include <ctype.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wchar.h>

static char phase[16];

/* pc.c sets PC_PHASE with setenv(); remember it in this process. */
int setenv(const char *name, const char *value, int overwrite)
{
	static int (*real)(const char *, const char *, int);

	if (real == NULL)
		real = (int (*)(const char *, const char *, int))dlsym(RTLD_NEXT, "setenv");
	if (strcmp(name, "PC_PHASE") == 0 && strlen(value) < sizeof phase)
		strcpy(phase, value);
	return real(name, value, overwrite);
}

static const char *mutant_of(const char *prefix)
{
	const char *m = getenv("PC_MUTANT");
	size_t n = strlen(prefix);

	if (strcmp(phase, "test") != 0 || m == NULL || strncmp(m, prefix, n) != 0)
		return NULL;
	return m + n;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)

typedef int (*vs_fn)(const char *, const char *, va_list);
typedef int (*vf_fn)(FILE *, const char *, va_list);

static vs_fn real_vs(void)
{
	static vs_fn f;

	if (f == NULL)
		f = (vs_fn)dlsym(RTLD_NEXT, "__isoc99_vsscanf");
	if (f == NULL)
		f = (vs_fn)dlsym(RTLD_NEXT, "vsscanf");
	return f;
}

static vf_fn real_vf(void)
{
	static vf_fn f;

	if (f == NULL)
		f = (vf_fn)dlsym(RTLD_NEXT, "__isoc99_vfscanf");
	if (f == NULL)
		f = (vf_fn)dlsym(RTLD_NEXT, "vfscanf");
	return f;
}

static int call_s(const char *s, const char *fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = real_vs()(s, fmt, ap);
	va_end(ap);
	return r;
}

static int call_f(FILE *f, const char *fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = real_vf()(f, fmt, ap);
	va_end(ap);
	return r;
}

/* ---------- the input: a string or a stream ---------- */

struct in {
	FILE *f;
	const char *s;
	size_t pos;
	long total;	/* bytes read so far, for %n */
};

static int in_get(struct in *in)
{
	int c;

	if (in->f != NULL)
		c = getc(in->f);
	else {
		c = (unsigned char)in->s[in->pos];
		if (c == 0)
			c = EOF;
		else
			in->pos++;
	}
	if (c != EOF)
		in->total++;
	return c;
}

static void in_unget(struct in *in, int c)
{
	if (c == EOF)
		return;
	if (in->f != NULL)
		(void)ungetc(c, in->f);
	else
		in->pos--;
	in->total--;
}

static int in_peek(struct in *in)
{
	int c = in_get(in);

	in_unget(in, c);
	return c;
}

static void skip_ws(struct in *in)
{
	int c;

	while ((c = in_get(in)) != EOF && isspace(c))
		;
	in_unget(in, c);
}

/* ---------- arguments ---------- */

struct args {
	va_list *ap;
	void *num[64];	/* numbered arguments, when the format uses %n$ */
	int numbered;
	void *held;	/* fault n_no_arg: an argument given out but not used up */
	int have_held;
};

static void *next_arg(struct args *a)
{
	if (a->have_held) {
		a->have_held = 0;
		return a->held;
	}
	return va_arg(*a->ap, void *);
}

/* ---------- one conversion specification ---------- */

struct spec {
	int suppress, width, m, argn;
	char len[3];
	char conv;
	char set[256];	/* for '[': the text after '[' up to and including ']' */
};

/* Parses the specification after '%'; returns the byte after it. */
static const char *parse_spec(const char *p, struct spec *sp)
{
	memset(sp, 0, sizeof *sp);
	if (isdigit((unsigned char)*p)) {
		const char *q = p;
		int n = 0;

		while (isdigit((unsigned char)*q))
			n = n * 10 + (*q++ - '0');
		if (*q == '$') {
			sp->argn = n;
			p = q + 1;
		}
	}
	if (*p == '*') {
		sp->suppress = 1;
		p++;
	}
	while (isdigit((unsigned char)*p))
		sp->width = sp->width * 10 + (*p++ - '0');
	if (*p == 'm') {
		sp->m = 1;
		p++;
	}
	if ((p[0] == 'h' && p[1] == 'h') || (p[0] == 'l' && p[1] == 'l')) {
		sp->len[0] = p[0];
		sp->len[1] = p[1];
		p += 2;
	} else if (strchr("hljztL", *p) != NULL && *p != '\0') {
		sp->len[0] = *p++;
	}
	sp->conv = *p;
	if (*p == '\0')
		return p;
	p++;
	if (sp->conv == '[') {
		size_t k = 0;

		if (*p == '^')
			sp->set[k++] = *p++;
		if (*p == ']')
			sp->set[k++] = *p++;
		while (*p != '\0' && *p != ']' && k < sizeof sp->set - 2)
			sp->set[k++] = *p++;
		if (*p == ']')
			sp->set[k++] = *p++;
		sp->set[k] = '\0';
	}
	return p;
}

static void build(char *out, size_t n, const struct spec *sp, int with_n)
{
	char w[16] = "";

	if (sp->width > 0)
		snprintf(w, sizeof w, "%d", sp->width);
	snprintf(out, n, "%%%s%s%s%s%c%s%s", sp->suppress ? "*" : "", w,
	    sp->m ? "m" : "", sp->len, sp->conv, sp->conv == '[' ? sp->set : "",
	    with_n ? "%n" : "");
}

static int is_float(char c)
{
	return c != '\0' && strchr("aeEfFgGA", c) != NULL;
}

static int is_int(char c)
{
	return c != '\0' && strchr("diouxX", c) != NULL;
}

static void store_count(void *t, const char *len, long v)
{
	if (strcmp(len, "hh") == 0)
		*(signed char *)t = (signed char)v;
	else if (strcmp(len, "h") == 0)
		*(short *)t = (short)v;
	else if (strcmp(len, "l") == 0)
		*(long *)t = v;
	else if (strcmp(len, "ll") == 0)
		*(long long *)t = v;
	else if (strcmp(len, "j") == 0)
		*(intmax_t *)t = v;
	else if (strcmp(len, "z") == 0)
		*(size_t *)t = (size_t)v;
	else if (strcmp(len, "t") == 0)
		*(ptrdiff_t *)t = v;
	else
		*(int *)t = (int)v;
}

static void store_float(void *t, const char *len, long double v)
{
	if (strcmp(len, "l") == 0)
		*(double *)t = (double)v;
	else if (strcmp(len, "L") == 0)
		*(long double *)t = v;
	else
		*(float *)t = (float)v;
}

static char pool[4096];
static size_t pool_used;

/* Results of one conversion. */
enum { OK, MATCH_FAIL, INPUT_FAIL };

/*
 * Runs one conversion (not %n, not %%) through the C library; t is the
 * target or NULL when suppressed.
 */
static int run_conv(const char *m, struct in *in, struct spec *sp, void *t)
{
	char fmt[300];
	int used = -1, r, res;
	struct spec real = *sp;
	union { int i; short s; float f; double d; } tmp;
	void *rt = t;

	if (IS("width_ignored"))
		real.width = 0;
	if (IS("d_as_i") && real.conv == 'd')
		real.conv = 'i';
	if (IS("i_as_d") && real.conv == 'i')
		real.conv = 'd';
	if (IS("o_as_u") && real.conv == 'o')
		real.conv = 'u';
	if (IS("p_decimal") && real.conv == 'p') {
		real.conv = 'u';
		strcpy(real.len, "l");
	}
	if (IS("upper_unknown") && strchr("AEFGX", real.conv) != NULL)
		return MATCH_FAIL;
	if (IS("xsi_cs_unknown") && (real.conv == 'C' || real.conv == 'S'))
		return MATCH_FAIL;
	if (IS("l_ignored") && strcmp(real.len, "l") == 0 && !real.m &&
	    strchr("sc[", real.conv) != NULL)
		real.len[0] = '\0';
	if (IS("scanset_no_leading_bracket") && real.conv == '[') {
		char *b = real.set + (real.set[0] == '^');

		if (b[0] == ']')
			memmove(b, b + 1, strlen(b));
	}
	if (IS("scanset_no_negate") && real.conv == '[' && real.set[0] == '^') {
		/* '^' becomes an ordinary member of the set, placed last */
		char body[250];
		size_t k;

		snprintf(body, sizeof body, "%s", real.set + 1);
		k = strlen(body);
		if (k > 0 && body[k - 1] == ']')
			body[k - 1] = '\0';
		snprintf(real.set, sizeof real.set, "%s^]", body);
	}
	if (t != NULL && IS("h_stores_int") && strcmp(real.len, "h") == 0 && is_int(real.conv)) {
		real.len[0] = '\0';
		rt = &tmp.i;
	}
	if (t != NULL && IS("l_stores_int") && strcmp(real.len, "l") == 0 && is_int(real.conv)) {
		real.len[0] = '\0';
		rt = &tmp.i;
	}
	if (t != NULL && IS("hh_stores_short") && strcmp(real.len, "hh") == 0 && is_int(real.conv)) {
		strcpy(real.len, "h");
		rt = &tmp.s;
	}
	if (t != NULL && IS("j_stores_int") && strcmp(real.len, "j") == 0 && is_int(real.conv)) {
		real.len[0] = '\0';
		rt = &tmp.i;
	}
	if (t != NULL && IS("lf_float_precision") && strcmp(real.len, "l") == 0 && is_float(real.conv)) {
		real.len[0] = '\0';
		rt = &tmp.f;
	}
	if (t != NULL && IS("ld_double_precision") && strcmp(real.len, "L") == 0 && is_float(real.conv)) {
		strcpy(real.len, "l");
		rt = &tmp.d;
	}
	if (IS("no_skip") && strchr("[cn", real.conv) == NULL && isspace(in_peek(in)))
		return MATCH_FAIL;
	if (IS("always_skip") && strchr("[c", real.conv) != NULL)
		skip_ws(in);
	if (IS("no_inf_nan") && is_float(real.conv)) {
		const char *q;

		if (in->f == NULL) {
			q = in->s + in->pos;
			while (isspace((unsigned char)*q))
				q++;
			if (*q == '-' || *q == '+')
				q++;
			if (strchr("iInN", *q) != NULL && *q != '\0')
				return MATCH_FAIL;
		}
	}
	if (IS("no_hex_float") && is_float(real.conv) && in->f == NULL) {
		const char *q = in->s + in->pos;

		while (isspace((unsigned char)*q))
			q++;
		if (q[0] == '0' && (q[1] == 'x' || q[1] == 'X')) {
			/* a C89-style scanner: the number is the "0" */
			in->total += (long)(q + 1 - (in->s + in->pos));
			in->pos = (size_t)(q + 1 - in->s);
			if (t != NULL)
				store_float(t, sp->len, 0);
			return OK;
		}
	}

	build(fmt, sizeof fmt, &real, 1);
	if (in->f != NULL)
		r = rt != NULL ? call_f(in->f, fmt, rt, &used) : call_f(in->f, fmt, &used);
	else
		r = rt != NULL ? call_s(in->s + in->pos, fmt, rt, &used) :
		    call_s(in->s + in->pos, fmt, &used);
	if (r == EOF)
		res = INPUT_FAIL;
	else if (rt != NULL)
		res = r == 1 ? OK : MATCH_FAIL;
	else
		res = used >= 0 ? OK : MATCH_FAIL;
	if (res == OK && used < 0)	/* %n not reached at end of input */
		used = in->f != NULL ? 0 : (int)strlen(in->s + in->pos);
	if (res == OK) {
		if (in->f == NULL)
			in->pos += (size_t)used;
		in->total += used;
	}

	if (res == MATCH_FAIL && IS("lone_sign") && is_int(sp->conv) && in->f == NULL) {
		const char *q = in->s + in->pos;

		while (isspace((unsigned char)*q))
			q++;
		if ((*q == '-' || *q == '+') && !isdigit((unsigned char)q[1])) {
			in->total += (long)(q + 1 - (in->s + in->pos));
			in->pos = (size_t)(q + 1 - in->s);
			if (t != NULL)
				store_count(t, sp->len, 0);
			return OK;
		}
	}
	if (res == MATCH_FAIL && IS("float_prefix_backtrack") && is_float(sp->conv) && in->f == NULL) {
		const char *q = in->s + in->pos;
		char *end;
		long double v = strtold(q, &end);

		if (end > q) {
			in->total += (long)(end - q);
			in->pos += (size_t)(end - q);
			if (t != NULL)
				store_float(t, sp->len, v);
			return OK;
		}
	}
	if (res == MATCH_FAIL && IS("hex_backtrack") && strchr("xXi", sp->conv) != NULL && in->f == NULL) {
		const char *q = in->s + in->pos;

		while (isspace((unsigned char)*q))
			q++;
		if (q[0] == '0' && (q[1] == 'x' || q[1] == 'X')) {
			in->total += (long)(q + 1 - (in->s + in->pos));
			in->pos = (size_t)(q + 1 - in->s);
			if (t != NULL)
				store_count(t, sp->len, 0);
			return OK;
		}
	}
	if (res == MATCH_FAIL && IS("scanset_empty_ok") && sp->conv == '[' && t != NULL && !sp->m) {
		*(char *)t = '\0';
		return OK;
	}
	if (res == MATCH_FAIL && IS("conflict_lost") && in_peek(in) != EOF)
		(void)in_get(in);
	if (res != OK)
		return res;

	/* the result is in; faults that bend the stored value */
	if (rt != t) {
		if (IS("h_stores_int") || IS("l_stores_int") || IS("j_stores_int"))
			memcpy(t, &tmp.i, sizeof tmp.i);
		else if (IS("hh_stores_short"))
			memcpy(t, &tmp.s, sizeof tmp.s);
		else if (IS("lf_float_precision"))
			*(double *)t = tmp.f;
		else if (IS("ld_double_precision"))
			*(long double *)t = tmp.d;
	}
	if (t != NULL && IS("float_truncates") && is_float(sp->conv) && sp->len[0] == '\0')
		*(float *)t = (float)(long)*(float *)t;
	if (t != NULL && IS("c_adds_null") && sp->conv == 'c' && !sp->m && sp->len[0] == '\0')
		((char *)t)[sp->width > 0 ? sp->width : 1] = '\0';
	if (t != NULL && IS("lc_adds_null") && sp->conv == 'c' && !sp->m && strcmp(sp->len, "l") == 0)
		((wchar_t *)t)[sp->width > 0 ? sp->width : 1] = L'\0';
	if (t != NULL && IS("s_no_null") && sp->conv == 's' && !sp->m && sp->len[0] == '\0')
		((char *)t)[strlen((char *)t)] = 'Z';
	if (t != NULL && IS("p_null_nonzero") && sp->conv == 'p' && *(void **)t == NULL)
		*(void **)t = (void *)1;
	if (t != NULL && sp->m && IS("m_not_malloc")) {
		/* the result moves to storage free() does not own */
		char *old = *(char **)t;
		size_t n = strcmp(sp->len, "l") == 0 ?
		    (sp->conv == 'c' ? (size_t)(sp->width > 0 ? sp->width : 1) : wcslen((wchar_t *)old) + 1) * sizeof(wchar_t) :
		    (sp->conv == 'c' ? (size_t)(sp->width > 0 ? sp->width : 1) : strlen(old) + 1);

		pool_used = (pool_used + 15) & ~(size_t)15;
		if (pool_used + n <= sizeof pool) {
			memcpy(pool + pool_used, old, n);
			*(char **)t = pool + pool_used;
			pool_used += n;
			free(old);
		}
	}
	if (t != NULL && sp->m && IS("ml_narrow") && strcmp(sp->len, "l") == 0 && sp->conv != 'c') {
		wchar_t *w = *(wchar_t **)t;
		size_t n = wcslen(w), i;
		char *c = malloc(n + 1);

		if (c != NULL) {
			for (i = 0; i <= n; i++)
				c[i] = (char)w[i];
			free(w);
			*(char **)t = c;
		}
	}
	if (IS("item_next_lost") && in_peek(in) != EOF)
		(void)in_get(in);
	return OK;
}

/* ---------- the driver ---------- */

static int drive(const char *m, struct in *in, const char *fmt, va_list *ap)
{
	struct args a;
	const char *p;
	int assigned = 0, convs = 0, c, numbered = 0;

	memset(&a, 0, sizeof a);
	a.ap = ap;
	/* numbered arguments: collect them first */
	for (p = fmt; *p != '\0'; p++) {
		if (*p == '%' && isdigit((unsigned char)p[1])) {
			const char *q = p + 1;
			int n = 0;

			while (isdigit((unsigned char)*q))
				n = n * 10 + (*q++ - '0');
			if (*q == '$' && n > numbered)
				numbered = n;
		}
	}
	if (numbered > 64)
		numbered = 64;
	if (numbered > 0 && !IS("numbered_ignored")) {
		int i;

		for (i = 0; i < numbered; i++)
			a.num[i] = va_arg(*ap, void *);
		a.numbered = 1;
	}

	for (p = fmt; *p != '\0';) {
		if (isspace((unsigned char)*p)) {
			int any;

			while (isspace((unsigned char)*p))
				p++;
			if (IS("ws_noop"))
				continue;
			c = in_peek(in);
			any = c != EOF && isspace(c);
			if (IS("ws_must_match") && !any)
				return assigned;
			skip_ws(in);
			if (IS("ws_eats_one") && in_peek(in) != EOF)
				(void)in_get(in);
			continue;
		}
		if (*p != '%' || p[1] == '%') {
			int want = (unsigned char)*p, pct = *p == '%';

			p += pct ? 2 : 1;
			if (pct && !IS("percent_no_skip"))
				skip_ws(in);
			c = in_get(in);
			if (c == EOF) {
				if (IS("ordinary_eof_zero"))
					return assigned;
				goto input_fail;
			}
			if (c != want) {
				if (!IS("mismatch_consumed"))
					in_unget(in, c);
				goto match_fail;
			}
			if (pct && IS("percent_counted"))
				assigned++;
			continue;
		}
		{
			struct spec sp;
			void *t = NULL;
			int r;

			p = parse_spec(p + 1, &sp);
			if (sp.conv == '\0')
				break;
			if (!sp.suppress) {
				if (a.numbered && sp.argn > 0 && sp.argn <= 64)
					t = a.num[sp.argn - 1];
				else
					t = next_arg(&a);
			} else if (IS("suppress_takes_arg") && !a.numbered) {
				(void)next_arg(&a);
			}
			if (sp.conv == 'n') {
				if (IS("always_skip"))
					skip_ws(in);
				if (t != NULL) {
					if (IS("n_stores_int"))
						*(int *)t = (int)in->total;
					else
						store_count(t, sp.len, in->total);
				}
				if (IS("n_counted"))
					assigned++;
				if (IS("n_no_arg") && !a.numbered) {
					a.held = t;
					a.have_held = 1;
				}
				continue;
			}
			r = run_conv(m, in, &sp, t);
			if (r == INPUT_FAIL)
				goto input_fail;
			if (r == MATCH_FAIL) {
				if (IS("count_failed"))
					assigned++;
				goto match_fail;
			}
			convs++;
			if (!sp.suppress && !(sp.m && IS("m_uncounted")))
				assigned++;
			else if (sp.suppress && IS("suppressed_counted"))
				assigned++;
		}
	}
	if (IS("eats_trailing_ws"))
		skip_ws(in);
	if (IS("writes_extra") && !a.numbered)
		*(int *)next_arg(&a) = 0;
	return assigned;

match_fail:
	if (IS("matching_returns_eof") && assigned == 0)
		return EOF;
	return assigned;

input_fail:
	if (IS("eof_returns_zero"))
		return assigned;
	if (IS("eof_always_eof"))
		return EOF;
	if (IS("eof_unless_assigned"))
		return assigned == 0 ? EOF : assigned;
	return convs == 0 ? EOF : assigned;
}

/* ---------- entry points ---------- */

static int scan(FILE *f, const char *s, const char *fmt, va_list ap)
{
	const char *m = mutant_of("fscanf.");
	struct in in;
	struct stat st0;
	int r, e, have_st = 0;
	va_list cp;

	if (m == NULL)
		return f != NULL ? real_vf()(f, fmt, ap) : real_vs()(s, fmt, ap);

	memset(&in, 0, sizeof in);
	in.f = f;
	in.s = s;

	if (IS("reads_stdin") && f != NULL && f != stdin)
		in.f = stdin;
	if (IS("own_buffer") && f == stdin) {
		/* reads descriptor 0 directly, behind the stream's back */
		static char buf[4096];
		ssize_t n = read(0, buf, sizeof buf - 1);

		buf[n > 0 ? n : 0] = '\0';
		in.f = NULL;
		in.s = buf;
	}
	if (IS("radix_comma") && s != NULL) {
		static char copy[4096];
		size_t i;

		snprintf(copy, sizeof copy, "%s", s);
		for (i = 0; copy[i] != '\0'; i++)
			if (copy[i] == ',')
				copy[i] = '.';
		in.s = copy;
	}
	if (IS("no_atime") && f != NULL && fstat(fileno(f), &st0) == 0)
		have_st = 1;

	va_copy(cp, ap);
	r = drive(m, &in, fmt, &cp);
	va_end(cp);
	e = errno;

	if (IS("tokenizes") && s != NULL) {
		/* cuts the string where the first item ended, as strtok() would */
		char *w = (char *)s + strcspn(s, " \t\n");

		if (*w != '\0')
			*w = '\0';
	}
	if (f != NULL && ferror(f)) {
		if (IS("error_returns_zero") && r == EOF)
			r = 0;
		if (IS("error_returns_eof"))
			r = EOF;
		if (IS("error_clears_indicator"))
			clearerr(f);
		if (IS("error_errno_zero"))
			e = 0;
	}
	if (have_st) {
		struct timespec ts[2];

		ts[0] = st0.st_atim;
		ts[1].tv_sec = 0;
		ts[1].tv_nsec = UTIME_OMIT;
		(void)futimens(fileno(f), ts);
	}
	errno = e;
	return r;
}

/*
 * The entry points get their symbol names from asm labels: under
 * _GNU_SOURCE, glibc's <stdio.h> renames sscanf() and the others to
 * __isoc23_* itself, so a plain definition would land on that name.
 */
#define ENTRY_S(fn, sym) \
int fn(const char *restrict s, const char *restrict fmt, ...) __asm__(sym); \
int fn(const char *restrict s, const char *restrict fmt, ...) \
{ \
	va_list ap; \
	int r; \
	va_start(ap, fmt); \
	r = scan(NULL, s, fmt, ap); \
	va_end(ap); \
	return r; \
}
#define ENTRY_F(fn, sym) \
int fn(FILE *restrict f, const char *restrict fmt, ...) __asm__(sym); \
int fn(FILE *restrict f, const char *restrict fmt, ...) \
{ \
	va_list ap; \
	int r; \
	va_start(ap, fmt); \
	r = scan(f, NULL, fmt, ap); \
	va_end(ap); \
	return r; \
}
#define ENTRY_IN(fn, sym) \
int fn(const char *restrict fmt, ...) __asm__(sym); \
int fn(const char *restrict fmt, ...) \
{ \
	va_list ap; \
	int r; \
	va_start(ap, fmt); \
	r = scan(stdin, NULL, fmt, ap); \
	va_end(ap); \
	return r; \
}

ENTRY_S(pc_sscanf, "sscanf")
ENTRY_S(pc_sscanf99, "__isoc99_sscanf")
ENTRY_S(pc_sscanf23, "__isoc23_sscanf")
ENTRY_F(pc_fscanf, "fscanf")
ENTRY_F(pc_fscanf99, "__isoc99_fscanf")
ENTRY_F(pc_fscanf23, "__isoc23_fscanf")
ENTRY_IN(pc_scanf, "scanf")
ENTRY_IN(pc_scanf99, "__isoc99_scanf")
ENTRY_IN(pc_scanf23, "__isoc23_scanf")
