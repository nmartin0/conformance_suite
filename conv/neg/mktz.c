/*
 * mktz - write small TZif fixtures, good and deliberately wrong, for the
 * negative checks of conv/tzif (conv/neg/run.sh).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * usage: mktz DIR
 *
 * Makes DIR/<case>/UTC and DIR/<case>/Zone for each case. Each case is a
 * database holding a valid UTC file and one Zone file with one planted
 * fault (or none, for the case "good"). The files follow RFC 8536 by hand.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

struct spec {
	int ntr;
	int64_t tr[8];
	unsigned char ty[8];
	int nty;
	int32_t utoff[4];
	unsigned char isdst[4];
	unsigned char abbrind[4];
	const char *chars;	/* with embedded NULs, charcnt bytes */
	int charcnt;
	const char *footer;
};

static void put32(unsigned char *p, uint32_t v)
{
	p[0] = (unsigned char)(v >> 24);
	p[1] = (unsigned char)(v >> 16);
	p[2] = (unsigned char)(v >> 8);
	p[3] = (unsigned char)v;
}

static size_t block(unsigned char *o, const struct spec *s, int tsize, int empty)
{
	size_t n = 0;
	int i;
	int ntr = empty ? 0 : s->ntr;

	memcpy(o + n, "TZif2", 5);
	memset(o + n + 5, 0, 15);
	put32(o + n + 20, 0);
	put32(o + n + 24, 0);
	put32(o + n + 28, 0);
	put32(o + n + 32, (uint32_t)ntr);
	put32(o + n + 36, (uint32_t)s->nty);
	put32(o + n + 40, (uint32_t)s->charcnt);
	n += 44;
	for (i = 0; i < ntr; i++) {
		if (tsize == 4) {
			put32(o + n, (uint32_t)s->tr[i]);
		} else {
			put32(o + n, (uint32_t)((uint64_t)s->tr[i] >> 32));
			put32(o + n + 4, (uint32_t)s->tr[i]);
		}
		n += (size_t)tsize;
	}
	for (i = 0; i < ntr; i++)
		o[n++] = s->ty[i];
	for (i = 0; i < s->nty; i++) {
		put32(o + n, (uint32_t)s->utoff[i]);
		o[n + 4] = s->isdst[i];
		o[n + 5] = s->abbrind[i];
		n += 6;
	}
	memcpy(o + n, s->chars, (size_t)s->charcnt);
	n += (size_t)s->charcnt;
	return n;
}

static void write_file(const char *dir, const char *name, const struct spec *s,
    void (*fault)(unsigned char *, size_t *))
{
	unsigned char b[2048];
	size_t n = 0;
	char path[512];
	FILE *f;

	n += block(b + n, s, 4, 1);	/* a version 1 block without transitions */
	n += block(b + n, s, 8, 0);
	b[n++] = '\n';
	memcpy(b + n, s->footer, strlen(s->footer));
	n += strlen(s->footer);
	b[n++] = '\n';
	if (fault != NULL)
		fault(b, &n);
	(void)snprintf(path, sizeof path, "%s/%s", dir, name);
	f = fopen(path, "wb");
	if (f == NULL || fwrite(b, 1, n, f) != n || fclose(f) != 0) {
		perror(path);
		exit(1);
	}
}

static void f_version(unsigned char *b, size_t *n) { (void)n; b[4] = 'X'; }
static void f_short(unsigned char *b, size_t *n) { (void)b; *n = 60; }
static void f_nonl(unsigned char *b, size_t *n) { (void)b; *n -= 1; }

static const struct spec utc = {
	0, { 0 }, { 0 }, 1, { 0 }, { 0 }, { 0 }, "UTC\0", 4, "UTC0"
};

static struct spec zone(void)
{
	struct spec s = {
		3, { 0, 1000000000, 2000000000 }, { 0, 1, 0 }, 2, { 3600, 7200 }, { 0, 1 }, { 0, 4 },
		"STD\0DST\0", 8, "STD-1"
	};

	return s;
}

static void make(const char *root, const char *name, struct spec z, void (*fault)(unsigned char *, size_t *))
{
	char d[512];

	(void)snprintf(d, sizeof d, "%s/%s", root, name);
	(void)mkdir(d, 0755);
	write_file(d, "UTC", &utc, NULL);
	write_file(d, "Zone", &z, fault);
}

int main(int argc, char **argv)
{
	struct spec z;

	if (argc != 2) {
		fprintf(stderr, "usage: mktz DIR\n");
		return 2;
	}
	(void)mkdir(argv[1], 0755);
	make(argv[1], "good", zone(), NULL);
	make(argv[1], "version", zone(), f_version);
	make(argv[1], "short", zone(), f_short);
	make(argv[1], "nofooternl", zone(), f_nonl);
	z = zone();
	z.tr[1] = 2000000000;
	z.tr[2] = 1000000000;		/* not ascending */
	make(argv[1], "unsorted", z, NULL);
	z = zone();
	z.ty[1] = 5;			/* type index beyond typecnt */
	make(argv[1], "typeidx", z, NULL);
	z = zone();
	z.footer = "XYZ-9";		/* disagrees with the last transition */
	make(argv[1], "footer", z, NULL);
	z = zone();
	z.utoff[0] = 100000;		/* outside the recommended range */
	make(argv[1], "utoff", z, NULL);
	return 0;
}
