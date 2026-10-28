/*
 * nullsys_rt.c - runtime of the negative control (the "null system").
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * Built with the generated nullsys.c into libnullsys.so by ci/nullsys.sh and
 * preloaded into each C test. It holds the test-phase switch, the call log,
 * the functions that need care (getenv, the allocators, confstr) and the
 * NaN and exit helpers the stubs use. See docs/negative-control.md.
 *
 * Rules for this file: it is compiled without optimisation and without
 * builtins, and it never calls a function of the C library by name (the
 * stubs would catch the call); real functions are fetched with ns_next().
 * The only library names it uses are dlsym, which is never stubbed, and the
 * names it defines itself.
 *
 * The switch. core/pc.c calls getenv("PC_NULLSYS_ON") when the test phase
 * begins and getenv("PC_NULLSYS_OFF") whenever its own helper code runs.
 * Nothing else reaches the stubs, so the harness (result writing, setup
 * helpers, exit) always runs on the real functions. Setup code of a test
 * runs with the switch off too, on the real system.
 *
 * Single-threaded by design: a test that starts threads while the switch is
 * off may race on the lazy lookups; the stubs refuse thread creation while
 * it is on.
 */
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef RTLD_NEXT
#error "this system's <dlfcn.h> has no RTLD_NEXT (try -D_GNU_SOURCE)"
#endif

extern char **environ;

#ifdef NS_POISON
# define NS_ERRNO 0x7ffffff0
#else
# define NS_ERRNO ENOSYS
#endif

#define NS_SEEN 4096
#define NS_RT_BASE 4000		/* indexes of the functions defined here */

static int ns_gate;		/* the test phase is on */
static int ns_internal;		/* the runtime itself is working */
static int ns_resolving;	/* a lookup is in progress */
static unsigned char ns_seen[NS_SEEN];
static char ns_id[160];		/* PC_ASSERTION at the moment of switching on */
static char ns_logpath[600];	/* PC_NULLSYS_LOG likewise */

/* ---------- small helpers that call nothing ---------- */

static int ns_eq(const char *a, const char *b)
{
	while (*a != '\0' && *a == *b) {
		a++;
		b++;
	}
	return *a == *b;
}

/* Value of an environment variable, read from environ directly. */
static const char *ns_env(const char *name)
{
	char **e;

	if (environ == NULL)
		return NULL;
	for (e = environ; *e != NULL; e++) {
		const char *p = *e, *n = name;

		while (*n != '\0' && *p == *n) {
			p++;
			n++;
		}
		if (*n == '\0' && *p == '=')
			return p + 1;
	}
	return NULL;
}

static void ns_copy(char *dst, size_t size, const char *src)
{
	size_t i = 0;

	if (src != NULL)
		for (; src[i] != '\0' && i + 1 < size; i++)
			dst[i] = src[i];
	dst[i] = '\0';
}

/* ---------- looking up the real functions ---------- */

void *ns_next(const char *name)
{
	void *p;

	ns_internal++;
	ns_resolving++;
	p = dlsym(RTLD_NEXT, name);
	ns_resolving--;
	ns_internal--;
	return p;
}

/*
 * Memory for the C library while it works for dlsym(): the allocators are
 * ours, and dlsym may call them before their real versions are found.
 */
static union {
	long double align;
	char bytes[1 << 16];
} ns_arena;
static size_t ns_arena_used;

#define NS_HDR 32

static void *ns_arena_alloc(size_t n)
{
	size_t need = (n + NS_HDR + 31) & ~(size_t)31;
	char *p;

	if (need < n || ns_arena_used + need > sizeof ns_arena.bytes) {
		errno = ENOMEM;
		return NULL;
	}
	p = ns_arena.bytes + ns_arena_used;
	ns_arena_used += need;
	*(size_t *)p = n;
	return p + NS_HDR;
}

static int ns_in_arena(const void *p)
{
	return (const char *)p >= ns_arena.bytes &&
	    (const char *)p < ns_arena.bytes + sizeof ns_arena.bytes;
}

/* ---------- the call log ---------- */

/*
 * One line "ID function" per function per process, appended with one
 * write(2) to $PC_NULLSYS_LOG. ID is $PC_ASSERTION as it was when the test
 * phase began. Any failure is ignored. The real open, write and close are
 * used and the descriptor is closed again at once, so the test's descriptor
 * numbering is not disturbed.
 */
static void ns_log(const char *name)
{
	static int (*r_open)(const char *, int, ...);
	static ssize_t (*r_write)(int, const void *, size_t);
	static int (*r_close)(int);
	char line[320];
	size_t n = 0, i;
	int fd, saved = errno;

	if (ns_logpath[0] == '\0' || ns_id[0] == '\0')
		return;
	ns_internal++;
	if (r_open == NULL)
		r_open = (int (*)(const char *, int, ...))ns_next("open");
	if (r_write == NULL)
		r_write = (ssize_t (*)(int, const void *, size_t))ns_next("write");
	if (r_close == NULL)
		r_close = (int (*)(int))ns_next("close");
	if (r_open != NULL && r_write != NULL && r_close != NULL) {
		for (i = 0; ns_id[i] != '\0' && n < 160; i++)
			line[n++] = ns_id[i];
		line[n++] = ' ';
		for (i = 0; name[i] != '\0' && n < 300; i++)
			line[n++] = name[i];
		line[n++] = '\n';
		fd = r_open(ns_logpath, O_WRONLY | O_APPEND | O_CREAT, 0666);
		if (fd >= 0) {
			(void)r_write(fd, line, n);
			(void)r_close(fd);
		}
	}
	ns_internal--;
	errno = saved;
}

/* ---------- the switch ---------- */

static void ns_on(void)
{
	ns_copy(ns_id, sizeof ns_id, ns_env("PC_ASSERTION"));
	ns_copy(ns_logpath, sizeof ns_logpath, ns_env("PC_NULLSYS_LOG"));
	ns_gate = 1;
}

/*
 * Called first by every stub. Returns non-zero when the stub is to fail:
 * the test phase is on and the call comes from the test (not from this
 * runtime). Logs the function the first time it is seen.
 */
int ns_enter(int idx, const char *name)
{
	if (!ns_gate || ns_internal)
		return 0;
	if (idx >= 0 && idx < NS_SEEN && !ns_seen[idx]) {
		ns_seen[idx] = 1;
		ns_log(name);
	}
	return 1;
}

void ns_seterr(void)
{
	errno = NS_ERRNO;
}

/*
 * A function that cannot return was called by the test: end the process
 * with a status (70) no test reports PASS with, without a result line.
 */
void ns_die(void)
{
	void (*r_exit)(int);

	ns_internal++;
	r_exit = (void (*)(int))ns_next("_exit");
	if (r_exit != NULL)
		r_exit(70);
	for (;;)
		;
}

/* ---------- failure values for floating results ---------- */

double ns_nan_d(void)
{
#ifdef NS_POISON
	union { double d; uint64_t u; } v;
	v.u = UINT64_C(0xFFF4000000000001);	/* sign set, quiet bit clear */
	return v.d;
#else
	volatile double z = 0.0;
	return z / z;
#endif
}

float ns_nan_f(void)
{
#ifdef NS_POISON
	union { float f; uint32_t u; } v;
	v.u = UINT32_C(0xFFA00001);
	return v.f;
#else
	volatile float z = 0.0f;
	return z / z;
#endif
}

long double ns_nan_ld(void)
{
#ifdef NS_POISON
	return (long double)ns_nan_d();
#else
	volatile long double z = 0.0L;
	return z / z;
#endif
}

double _Complex ns_nan_dc(void)
{
	double _Complex z;
	double *p = (double *)&z;

	p[0] = ns_nan_d();
	p[1] = ns_nan_d();
	return z;
}

float _Complex ns_nan_fc(void)
{
	float _Complex z;
	float *p = (float *)&z;

	p[0] = ns_nan_f();
	p[1] = ns_nan_f();
	return z;
}

long double _Complex ns_nan_ldc(void)
{
	long double _Complex z;
	long double *p = (long double *)&z;

	p[0] = ns_nan_ld();
	p[1] = ns_nan_ld();
	return z;
}

/* ---------- getenv: carries the switch ---------- */

/*
 * The library is preloaded into the test only: the first time the test
 * asks for its environment (pc_begin() does, before any setup) the
 * LD_PRELOAD entry is taken out of it, so that programs the test's setup
 * starts run on the real system, not through the stubs.
 */
static void ns_strip_preload(void)
{
	static int done;
	char **e, **w;

	if (done || environ == NULL)
		return;
	done = 1;
	for (e = w = environ; *e != NULL; e++) {
		const char *k = "LD_PRELOAD=", *p = *e;

		while (*k != '\0' && *p == *k) {
			p++;
			k++;
		}
		if (*k != '\0')
			*w++ = *e;
	}
	*w = NULL;
}

char *getenv(const char *name)
{
	static char *(*real)(const char *);

	ns_strip_preload();
	if (name[0] == 'P' && name[1] == 'C' && name[2] == '_' && name[3] == 'N') {
		if (ns_eq(name, "PC_NULLSYS_ON")) {
			ns_on();
			return NULL;
		}
		if (ns_eq(name, "PC_NULLSYS_OFF")) {
			ns_gate = 0;
			return NULL;
		}
	}
	if (ns_enter(NS_RT_BASE + 0, "getenv")) {
		ns_seterr();
		return NULL;
	}
	if (real == NULL)
		real = (char *(*)(const char *))ns_next("getenv");
	if (real == NULL)
		return (char *)ns_env(name);
	return real(name);
}

/* ---------- allocators ---------- */

void *malloc(size_t n)
{
	static void *(*real)(size_t);

	if (ns_enter(NS_RT_BASE + 1, "malloc")) {
		ns_seterr();
		return NULL;
	}
	if (ns_resolving)
		return ns_arena_alloc(n);
	if (real == NULL)
		real = (void *(*)(size_t))ns_next("malloc");
	if (real == NULL)
		return ns_arena_alloc(n);
	return real(n);
}

void *calloc(size_t count, size_t size)
{
	static void *(*real)(size_t, size_t);

	if (ns_enter(NS_RT_BASE + 2, "calloc")) {
		ns_seterr();
		return NULL;
	}
	if (ns_resolving || real == NULL) {
		size_t total = count * size;

		if (count != 0 && total / count != size) {
			errno = ENOMEM;
			return NULL;
		}
		if (!ns_resolving) {
			real = (void *(*)(size_t, size_t))ns_next("calloc");
			if (real != NULL)
				return real(count, size);
		}
		return ns_arena_alloc(total);	/* the arena starts zeroed */
	}
	return real(count, size);
}

void *realloc(void *p, size_t n)
{
	static void *(*real)(void *, size_t);

	if (ns_enter(NS_RT_BASE + 3, "realloc")) {
		ns_seterr();
		return NULL;
	}
	if (p != NULL && ns_in_arena(p)) {
		size_t old = *(size_t *)((char *)p - NS_HDR), i;
		void *q = (ns_resolving || real == NULL) ? ns_arena_alloc(n)
		    : real(NULL, n);

		if (q != NULL)
			for (i = 0; i < old && i < n; i++)
				((char *)q)[i] = ((char *)p)[i];
		return q;
	}
	if (ns_resolving)
		return ns_arena_alloc(n);
	if (real == NULL)
		real = (void *(*)(void *, size_t))ns_next("realloc");
	if (real == NULL)
		return ns_arena_alloc(n);
	return real(p, n);
}

void free(void *p)
{
	static void (*real)(void *);

	if (p == NULL || ns_in_arena(p))
		return;
	if (ns_enter(NS_RT_BASE + 4, "free"))
		return;		/* leaks, on purpose */
	if (ns_resolving)
		return;
	if (real == NULL)
		real = (void (*)(void *))ns_next("free");
	if (real != NULL)
		real(p);
}

/* ---------- confstr: the stub utilities first on the path ---------- */

/*
 * Outside the test phase confstr(_CS_PATH) gives $PC_NULLSYS_BIN (the
 * directory of stub utilities) in front of the real value, so that setup
 * code and anything else the null system's "installation" finds looks there
 * first. Everything else, and every call in the test phase, is as for any
 * other stub.
 */
size_t confstr(int name, char *buf, size_t len)
{
	static size_t (*real)(int, char *, size_t);
	static char tmp[4096];
	const char *bin;
	size_t n, b = 0, i, total;

	if (ns_enter(NS_RT_BASE + 5, "confstr")) {
		ns_seterr();
		return (size_t)-1;
	}
	if (real == NULL)
		real = (size_t (*)(int, char *, size_t))ns_next("confstr");
	if (real == NULL) {
		ns_seterr();
		return 0;
	}
	bin = ns_env("PC_NULLSYS_BIN");
	if (name != _CS_PATH || bin == NULL || bin[0] == '\0')
		return real(name, buf, len);
	n = real(name, tmp, sizeof tmp);
	if (n == 0 || n > sizeof tmp)
		return real(name, buf, len);
	while (bin[b] != '\0')
		b++;
	total = b + 1 + n;		/* bin ":" value NUL */
	if (buf != NULL && len > 0) {
		size_t k = 0;

		for (i = 0; i < b && k + 1 < len; i++)
			buf[k++] = bin[i];
		if (k + 1 < len)
			buf[k++] = ':';
		for (i = 0; tmp[i] != '\0' && k + 1 < len; i++)
			buf[k++] = tmp[i];
		buf[k] = '\0';
	}
	return total;
}
