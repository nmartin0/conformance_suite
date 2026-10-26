/*
 * fprintf.c - planted faults in the fprintf() family, for mutation checks
 * (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "fprintf.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process or a parent).
 * While one is active, every function of the family formats through
 * fmt.inc, which leaves each conversion to the C library except the one
 * the fault breaks.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
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
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

#include "fmt.inc"

static const char *m;	/* the active fault, for the hooks */

static int is_inf(double d)
{
	return d == d && d - d != d - d;
}

static int is_nan(double d)
{
	return d != d;
}

static double as_double(const struct fe_spec *sp, union fe_val v)
{
	return sp->type == FE_LDBL ? (double)v.ld : v.d;
}

static int is_signed_conv(char c)
{
	return c != '\0' && strchr("difFeEgGaA", c) != NULL;
}

static int wide(const struct fe_spec *sp)
{
	return sp->conv == 'C' || sp->conv == 'S' || strcmp(sp->len, "l") == 0;
}

/* ---------- hooks ---------- */

static int fe_hook_text(struct fe_out *o, const char *s, size_t n, int last)
{
	if (IS("drop_last_ordinary") && last) {
		fe_put(o, s, n - 1);	/* an off-by-one at the end of the format */
		return 1;
	}
	return 0;
}

static int fe_hook_percent(struct fe_out *o, struct fe_args *a)
{
	if (IS("percent_consumes")) {
		(void)fe_arg(a, 0, FE_INT);	/* treats %% as a conversion */
		fe_put(o, "%", 1);
		return 1;
	}
	if (IS("percent_doubled")) {
		fe_put(o, "%%", 2);
		return 1;
	}
	return 0;
}

static void fe_hook_stars(struct fe_spec *sp, int *w, int *p)
{
	if (IS("star_neg_width") && sp->wstar && *w < 0)
		*w = -*w;	/* forgets that a negative width means '-' */
	if (IS("star_neg_prec") && sp->pstar && *p < 0)
		*p = 0;		/* a negative precision taken as zero */
	if (IS("star_swapped") && sp->wstar && sp->pstar) {
		int t = *w;	/* precision fetched before the width */

		*w = *p;
		*p = t;
	}
}

static void fe_hook_spec(struct fe_spec *sp, union fe_val *v)
{
	char c = sp->conv;
	int in = fe_isint(c), fl = fe_isfloat(c);

	if (IS("i_unsigned") && c == 'i') { sp->conv = 'u'; sp->type = FE_UINT; }
	if (IS("x_upper") && c == 'x') sp->conv = 'X';
	if (IS("X_lower") && c == 'X') sp->conv = 'x';
	if (IS("o_decimal") && c == 'o') sp->conv = 'u';
	if (IS("prec_ignored") && in) sp->prec = -1;
	if (IS("zero_prec_digit") && in && sp->prec == 0 && v->u == 0) sp->prec = 1;
	if (IS("width_ignored")) sp->width = -1;
	if (IS("width_truncates") && sp->width > 0) sp->mark = 1;
	if (IS("minus_ignored")) sp->minus = 0;
	if (IS("plus_ignored")) sp->plus = 0;
	if (IS("plus_default") && is_signed_conv(c)) sp->plus = 1;
	if (IS("space_ignored")) sp->space = 0;
	if (IS("space_beats_plus") && sp->space && sp->plus) sp->plus = 0;
	if (IS("alt_o_ignored") && c == 'o') sp->alt = 0;
	if (IS("alt_o_extra") && c == 'o' && sp->alt) { sp->alt = 0; sp->mark = 1; }
	if (IS("alt_x_ignored") && (c == 'x' || c == 'X')) sp->alt = 0;
	if (IS("alt_x_zero") && (c == 'x' || c == 'X') && sp->alt && v->u == 0) sp->mark = 1;
	if (IS("zero_before_sign") && in && sp->zero && !sp->minus && sp->prec < 0) { sp->zero = 0; sp->mark = 1; }
	if (IS("zero_ignored") && in) sp->zero = 0;
	if (IS("zero_with_minus") && sp->zero && sp->minus) sp->minus = 0;
	if (IS("zero_with_prec") && in && sp->zero && sp->prec >= 0 && sp->width > 0) sp->prec = -1;
	if (IS("zero_float_ignored") && fl) sp->zero = 0;
	if (IS("zero_pads_inf") && fl && sp->zero && !sp->minus) {
		double d = as_double(sp, *v);

		if (is_inf(d) || is_nan(d)) { sp->zero = 0; sp->mark = 1; }
	}
	if (IS("hh_ignored") && strcmp(sp->len, "hh") == 0) sp->len[0] = sp->len[1] = '\0';
	if (IS("hh_unsigned") && strcmp(sp->len, "hh") == 0 && (c == 'd' || c == 'i')) { sp->conv = 'u'; sp->type = FE_UINT; }
	if (IS("h_ignored") && strcmp(sp->len, "h") == 0) sp->len[0] = '\0';
	if (in && ((IS("l_truncated") && strcmp(sp->len, "l") == 0) ||
	    (IS("ll_truncated") && strcmp(sp->len, "ll") == 0) ||
	    (IS("j_truncated") && strcmp(sp->len, "j") == 0) ||
	    (IS("z_truncated") && strcmp(sp->len, "z") == 0) ||
	    (IS("t_truncated") && strcmp(sp->len, "t") == 0))) {
		/* converted as if the argument were an int */
		sp->len[0] = sp->len[1] = '\0';
		sp->type = (c == 'd' || c == 'i') ? FE_INT : FE_UINT;
	}
	if (IS("s_prec_ignored") && c == 's' && !wide(sp)) sp->prec = -1;
	if (IS("s_strlen") && c == 's' && !wide(sp) && sp->prec >= 0 && v->p != NULL) {
		volatile size_t n = strlen((const char *)v->p);	/* reads to the null byte */
		(void)n;
	}
	if (IS("f_default_prec5") && (c == 'f' || c == 'F') && sp->prec < 0) sp->prec = 5;
	if (IS("f_zero_prec_radix") && (c == 'f' || c == 'F') && sp->prec == 0) sp->alt = 1;
	if (IS("e_case") && c == 'E') sp->conv = 'e';
	if (IS("g_keeps_zeros") && (c == 'g' || c == 'G')) sp->alt = 1;
	if (IS("g_switch_off") && (c == 'g' || c == 'G') && !sp->alt && sp->type == FE_DBL &&
	    !is_inf(v->d) && !is_nan(v->d)) {
		/* the f style chosen only for P > X + 1 */
		int P = sp->prec < 0 ? 6 : sp->prec == 0 ? 1 : sp->prec, X;
		char b[64], *e;

		(void)fe_real()(b, sizeof b, "%.*e", P - 1, v->d);
		e = strchr(b, 'e');
		X = e ? atoi(e + 1) : 0;
		if (X == P - 1) {
			sp->conv = c == 'g' ? 'e' : 'E';
			sp->prec = P - 1;
		}
	}
	if (IS("alt_float_ignored") && fl && c != 'g' && c != 'G') sp->alt = 0;
	if (IS("alt_g_strips") && (c == 'g' || c == 'G')) sp->alt = 0;
	if (IS("a_case") && c == 'A') sp->conv = 'a';
	if (IS("a_prec_ignored") && (c == 'a' || c == 'A')) sp->prec = -1;
	if (IS("a_truncates") && (c == 'a' || c == 'A') && sp->prec >= 0) {
		sp->keep = sp->prec;	/* converted exactly, then cut */
		sp->prec = -1;
		sp->mark = 1;
	}
	if (IS("F_lowercase") && c == 'F') sp->conv = 'f';
	if (IS("ls_prec_ignored") && c == 's' && wide(sp)) sp->prec = -1;
	if (IS("ls_overread") && c == 's' && wide(sp) && sp->prec >= 0 && v->p != NULL) {
		volatile size_t n = wcslen((const wchar_t *)v->p);	/* reads to the null wide character */
		(void)n;
	}
	if (IS("S_as_s") && c == 'S') { sp->conv = 's'; sp->len[0] = '\0'; }
}

/* Replaces the leading spaces of piece with zeros. */
static void zeros_for_spaces(struct fe_out *o, const char *piece, size_t n)
{
	size_t i;

	for (i = 0; i < n && piece[i] == ' '; i++)
		fe_put(o, "0", 1);
	fe_put(o, piece + i, n - i);
}

static void upper(struct fe_out *o, const char *piece, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++) {
		char ch = piece[i];

		if (ch >= 'a' && ch <= 'z')
			ch = (char)(ch - 'a' + 'A');
		fe_put(o, &ch, 1);
	}
}

/* Inserts ',' between groups of three in the first run of digits. */
static void grouped(struct fe_out *o, const char *piece, size_t n)
{
	size_t i = 0, j, d;

	while (i < n && (piece[i] < '0' || piece[i] > '9'))
		i++;
	fe_put(o, piece, i);
	for (j = i; j < n && piece[j] >= '0' && piece[j] <= '9'; j++)
		;
	for (d = i; d < j; d++) {
		fe_put(o, piece + d, 1);
		if (d + 1 < j && (j - d - 1) % 3 == 0)
			fe_put(o, ",", 1);
	}
	fe_put(o, piece + j, n - j);
}

/* A hexadecimal floating piece with its fraction cut to keep digits. */
static void cut_hex(struct fe_out *o, const char *piece, size_t n, int keep, int alt)
{
	const char *dot = memchr(piece, '.', n), *p = piece;
	size_t i, k;

	for (i = 0; i < n && piece[i] != 'p' && piece[i] != 'P'; i++)
		;
	if (dot == NULL)
		dot = piece + i;	/* no fraction at all */
	fe_put(o, p, (size_t)(dot - p));
	if (keep > 0 || alt)
		fe_put(o, ".", 1);
	for (k = 0; k < (size_t)keep; k++) {
		const char *d = dot + 1 + k;

		fe_put(o, d < piece + i ? d : "0", 1);
	}
	fe_put(o, piece + i, n - i);
}

static int fe_hook_piece(struct fe_out *o, const struct fe_spec *sp, union fe_val v,
    const char *piece, size_t n)
{
	char c = sp->conv;

	if (IS("width_truncates") && sp->mark && n > (size_t)sp->width) {
		fe_put(o, piece, (size_t)sp->width);
		return 1;
	}
	if (IS("alt_o_extra") && sp->mark) {
		fe_put(o, "0", 1);	/* a zero in front, needed or not */
		fe_put(o, piece, n);
		return 1;
	}
	if (IS("alt_x_zero") && sp->mark) {
		fe_put(o, c == 'x' ? "0x0" : "0X0", 3);
		return 1;
	}
	if ((IS("zero_before_sign") || IS("zero_pads_inf")) && sp->mark) {
		zeros_for_spaces(o, piece, n);
		return 1;
	}
	if (IS("neg_abs") && (c == 'd' || c == 'i') && n > 0) {
		size_t i;

		for (i = 0; i < n; i++)
			if (piece[i] != '-')
				fe_put(o, piece + i, 1);
		return 1;
	}
	if (IS("c_nul_dropped") && c == 'c' && !wide(sp) && (unsigned char)v.i == 0) {
		size_t i;

		for (i = 0; i < n; i++)
			if (piece[i] != '\0')
				fe_put(o, piece + i, 1);
		return 1;
	}
	if (IS("p_raw") && c == 'p') {
		fe_put(o, (const char *)&v.p, sizeof v.p);	/* the bytes, not characters */
		return 1;
	}
	if (IS("e_one_digit_exp") && (c == 'e' || c == 'E')) {
		const char *x = NULL;
		size_t i;

		for (i = 0; i + 3 < n + 1; i++)
			if ((piece[i] == 'e' || piece[i] == 'E') && i + 2 < n &&
			    (piece[i + 1] == '+' || piece[i + 1] == '-') && piece[i + 2] == '0')
				x = piece + i + 2;
		if (x != NULL) {
			fe_put(o, piece, (size_t)(x - piece));
			fe_put(o, x + 1, n - (size_t)(x - piece) - 1);
			return 1;
		}
	}
	if (IS("a_exp_padded") && (c == 'a' || c == 'A')) {
		size_t i;

		for (i = 0; i + 2 < n; i++)
			if ((piece[i] == 'p' || piece[i] == 'P') &&
			    (i + 3 == n || piece[i + 3] < '0' || piece[i + 3] > '9')) {
				fe_put(o, piece, i + 2);
				fe_put(o, "0", 1);	/* two exponent digits */
				fe_put(o, piece + i + 2, n - i - 2);
				return 1;
			}
	}
	if (IS("a_truncates") && sp->mark) {
		cut_hex(o, piece, n, sp->keep, sp->alt);
		return 1;
	}
	if (fe_isfloat(c) && (c == 'f' || c == 'e' || c == 'g' || c == 'a')) {
		double d = as_double(sp, v);

		if ((IS("inf_case") && is_inf(d)) || (IS("nan_case") && is_nan(d))) {
			upper(o, piece, n);
			return 1;
		}
		if (IS("e_inf_mixed") && c == 'e' && is_inf(d)) {
			fe_put(o, d < 0 ? "-infinity" : "infinity", d < 0 ? 9 : 8);
			return 1;
		}
	}
	if (IS("apos_comma") && sp->apos && strchr("diufFgG", c) != NULL) {
		grouped(o, piece, n);
		return 1;
	}
	if (IS("radix_comma") && fe_isfloat(c)) {
		size_t i;

		for (i = 0; i < n; i++)
			fe_put(o, piece[i] == '.' ? "," : piece + i, 1);
		return 1;
	}
	if ((c == 'c' || c == 'C') && wide(sp)) {
		if (IS("lc_dropped") || (IS("lc_nul_empty") && v.wc == 0))
			return 1;
		if (IS("lc_mb_first_byte")) {
			char mb[MB_LEN_MAX];
			mbstate_t st;
			size_t k;

			memset(&st, 0, sizeof st);
			k = wcrtomb(mb, (wchar_t)v.wc, &st);
			if (k != (size_t)-1 && k > 1) {
				fe_put(o, mb, 1);
				return 1;
			}
		}
	}
	if (IS("C_literal") && c == 'C') {
		fe_put(o, "%C", 2);
		return 1;
	}
	if (IS("ls_partial") && c == 's' && wide(sp) && sp->prec >= 0 && v.p != NULL) {
		const wchar_t *ws = v.p;
		char mb[MB_LEN_MAX];
		mbstate_t st;
		size_t k, total = 0;

		memset(&st, 0, sizeof st);
		for (; *ws != 0 && total < (size_t)sp->prec; ws++) {
			k = wcrtomb(mb, *ws, &st);
			if (k == (size_t)-1)
				break;
			if (total + k > (size_t)sp->prec)
				k = (size_t)sp->prec - total;	/* the first bytes of a character */
			fe_put(o, mb, k);
			total += k;
		}
		return 1;
	}
	return 0;
}

static int fe_hook_fail(struct fe_out *o, const struct fe_spec *sp, union fe_val v)
{
	(void)o;
	if (IS("ilseq_skipped") && wide(sp)) {
		if (sp->conv == 's' || sp->conv == 'S') {
			const wchar_t *ws = v.p;
			char mb[MB_LEN_MAX];
			mbstate_t st;
			size_t k;

			memset(&st, 0, sizeof st);
			for (; ws != NULL && *ws != 0; ws++) {
				k = wcrtomb(mb, *ws, &st);
				if (k == (size_t)-1) {
					memset(&st, 0, sizeof st);
					continue;	/* skipped, no error */
				}
				fe_put(o, mb, k);
			}
		}
		return 1;
	}
	return 0;
}

static void fe_hook_n(struct fe_out *o, struct fe_spec *sp, size_t *count)
{
	if (IS("n_off_by_one"))
		*count += 1;
	if (IS("n_prints")) {
		char b[32];
		int k = fe_real()(b, sizeof b, "%zu", *count);

		fe_put(o, b, (size_t)k);	/* converts the count as an argument */
	}
	if (IS("n_hh_as_int") && (strcmp(sp->len, "hh") == 0 || strcmp(sp->len, "h") == 0))
		sp->len[0] = sp->len[1] = '\0';	/* stores a whole int */
}

static void fe_hook_type(struct fe_spec *sp)
{
	if (!fe_isfloat(sp->conv))
		return;
	if (IS("L_as_double") && sp->type == FE_LDBL) {
		sp->type = FE_DBL;	/* fetched and converted as a double */
		sp->len[0] = '\0';
	} else if (IS("lf_long_double") && strcmp(sp->len, "l") == 0) {
		sp->type = FE_LDBL;	/* 'l' taken as 'L' */
		sp->len[0] = 'L';
	}
}

static int fe_hook_posmode(void)
{
	return (IS("pos_ignored") ? 1 : 0) | (IS("pos_star_ignored") ? 2 : 0);
}

/* ---------- the functions ---------- */

/* Formats with the active fault; -1 with errno on error. */
static int run(const char *fmt, va_list ap, char **res, size_t *len)
{
	va_list cp;
	int r, e;

	va_copy(cp, ap);
	e = errno;
	r = fe_run(fmt, &cp, res, len);
	va_end(cp);
	if (r != 0 && (IS("overflow_no_errno") || IS("overflow_clamps")) && errno == EOVERFLOW) {
		errno = e;
		if (IS("overflow_clamps")) {
			*res = calloc(1, 1);
			*len = 0;
			return INT_MAX;	/* a count that is no longer the length */
		}
	}
	return r;
}

static int ret(int r, size_t len)
{
	return r == INT_MAX ? INT_MAX : (int)len;
}

static int do_vsnprintf(char *s, size_t n, const char *fmt, va_list ap)
{
	char *b;
	size_t len, k;
	int r = run(fmt, ap, &b, &len);

	if (r < 0)
		return -1;
	if (n == 0) {
		if (IS("sn_zero_writes") && s != NULL)
			s[0] = '\0';
		free(b);
		return IS("sn_zero_returns0") ? 0 : ret(r, len);
	}
	k = len < n - 1 ? len : n - 1;
	if (IS("sn_overrun") && len >= n) {
		k = n;		/* one byte too many, then the null byte */
		memcpy(s, b, k);
		s[k] = '\0';
	} else {
		memcpy(s, b, k);
		if (!(IS("sn_no_nul") && len >= n))
			s[k] = '\0';
	}
	free(b);
	if (IS("sn_returns_written"))
		return (int)k;
	return ret(r, len);
}

static int do_vsprintf(char *s, const char *fmt, va_list ap)
{
	char *b;
	size_t len;
	int r = run(fmt, ap, &b, &len);

	if (r < 0)
		return -1;
	memcpy(s, b, len);
	if (!IS("sprintf_no_nul"))
		s[len] = '\0';
	free(b);
	return IS("sprintf_counts_nul") ? (int)len + 1 : ret(r, len);
}

static struct timespec saved[2];
static int saved_fd = -1;

static int do_vfprintf(FILE *f, const char *fmt, va_list ap, int is_printf)
{
	char *b;
	size_t len, w;
	int r = run(fmt, ap, &b, &len);

	if (r < 0)
		return -1;
	if (is_printf && IS("printf_to_stderr"))
		f = stderr;
	if (IS("fprintf_no_times")) {
		struct stat st;

		if (fstat(fileno(f), &st) == 0) {
			saved[0] = st.st_atim;
			saved[1] = st.st_mtim;
			saved_fd = fileno(f);
		}
	}
	if (IS("fprintf_stops_at_nul"))
		w = fwrite(b, 1, strlen(b), f) < strlen(b) ? 0 : len;
	else if (IS("fprintf_bypasses_buffer"))
		w = write(fileno(f), b, len) == (ssize_t)len ? len : 0;
	else
		w = fwrite(b, 1, len, f);
	free(b);
	if (w < len && !IS("fprintf_ignores_error"))
		return -1;
	if (IS("fprintf_ignores_error"))
		clearerr(f);
	if ((is_printf && IS("printf_returns_zero")) || (!is_printf && IS("fprintf_returns_zero")))
		return 0;
	return ret(r, len);
}

static int do_vdprintf(int fd, const char *fmt, va_list ap)
{
	char *b;
	size_t len, done = 0;
	struct stat st;
	int r = run(fmt, ap, &b, &len), have = 0;

	if (r < 0)
		return -1;
	if (IS("dprintf_to_stdout"))
		fd = 1;
	if (IS("dprintf_no_times") && fstat(fd, &st) == 0)
		have = 1;
	while (!IS("dprintf_lost") && done < len) {
		ssize_t k = write(fd, b + done, len - done);

		if (k < 0) {
			if (errno == EINTR)
				continue;
			free(b);
			if (IS("dprintf_ignores_error"))
				return (int)len;
			if (IS("dprintf_errno"))
				errno = EIO;
			return -1;
		}
		done += (size_t)k;
	}
	free(b);
	if (have) {
		struct timespec t[2];

		t[0] = st.st_atim;
		t[1] = st.st_mtim;
		(void)futimens(fd, t);	/* the times left as they were */
	}
	return IS("dprintf_returns_zero") ? 0 : ret(r, len);
}

static int do_vasprintf(char **ptr, const char *fmt, va_list ap)
{
	char *b;
	size_t len;
	int r = run(fmt, ap, &b, &len);

	if (r < 0)
		return -1;
	if (IS("asprintf_truncates") && len > 1024) {
		len = 1024;
		b[len] = '\0';
	}
	*ptr = b;
	return IS("asprintf_returns_size") ? (int)len + 1 : ret(r, len);
}

int vsnprintf(char *restrict s, size_t n, const char *restrict fmt, va_list ap)
{
	REAL(vsnprintf);

	m = mutant_of("fprintf.");
	if (m == NULL)
		return real_vsnprintf(s, n, fmt, ap);
	return do_vsnprintf(s, n, fmt, ap);
}

int snprintf(char *restrict s, size_t n, const char *restrict fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = vsnprintf(s, n, fmt, ap);
	va_end(ap);
	return r;
}

int vsprintf(char *restrict s, const char *restrict fmt, va_list ap)
{
	REAL(vsprintf);

	m = mutant_of("fprintf.");
	if (m == NULL)
		return real_vsprintf(s, fmt, ap);
	return do_vsprintf(s, fmt, ap);
}

int sprintf(char *restrict s, const char *restrict fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = vsprintf(s, fmt, ap);
	va_end(ap);
	return r;
}

int vfprintf(FILE *restrict f, const char *restrict fmt, va_list ap)
{
	REAL(vfprintf);

	m = mutant_of("fprintf.");
	if (m == NULL)
		return real_vfprintf(f, fmt, ap);
	return do_vfprintf(f, fmt, ap, 0);
}

int fprintf(FILE *restrict f, const char *restrict fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = vfprintf(f, fmt, ap);
	va_end(ap);
	return r;
}

int vprintf(const char *restrict fmt, va_list ap)
{
	REAL(vprintf);

	m = mutant_of("fprintf.");
	if (m == NULL)
		return real_vprintf(fmt, ap);
	return do_vfprintf(stdout, fmt, ap, 1);
}

int printf(const char *restrict fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = vprintf(fmt, ap);
	va_end(ap);
	return r;
}

int vdprintf(int fd, const char *restrict fmt, va_list ap)
{
	REAL(vdprintf);

	m = mutant_of("fprintf.");
	if (m == NULL)
		return real_vdprintf(fd, fmt, ap);
	return do_vdprintf(fd, fmt, ap);
}

int dprintf(int fd, const char *restrict fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = vdprintf(fd, fmt, ap);
	va_end(ap);
	return r;
}

int vasprintf(char **restrict ptr, const char *restrict fmt, va_list ap)
{
	REAL(vasprintf);

	m = mutant_of("fprintf.");
	if (m == NULL)
		return real_vasprintf(ptr, fmt, ap);
	return do_vasprintf(ptr, fmt, ap);
}

int asprintf(char **restrict ptr, const char *restrict fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = vasprintf(ptr, fmt, ap);
	va_end(ap);
	return r;
}

/* For "fprintf_no_times": the times written back after the stream's data is. */
static void put_back(FILE *f)
{
	if (saved_fd >= 0 && (f == NULL || fileno(f) == saved_fd))
		(void)futimens(saved_fd, saved);
}

int fflush(FILE *f)
{
	REAL(fflush);
	int r;

	m = mutant_of("fprintf.");
	r = real_fflush(f);
	if (IS("fprintf_no_times"))
		put_back(f);
	return r;
}

int fclose(FILE *f)
{
	REAL(fclose);
	REAL(fflush);
	int fd = fileno(f), r;

	m = mutant_of("fprintf.");
	if (IS("fprintf_no_times") && fd == saved_fd) {
		(void)real_fflush(f);
		put_back(f);
	}
	r = real_fclose(f);
	return r;
}
