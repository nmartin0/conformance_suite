/*
 * tzif.h - a reader and checker for TZif files (RFC 8536), for the tests of
 * conv/tzif. No library function reads the files: the parser is here.
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * Include after "pc.h" and "cv.h". All functions are static.
 *
 * Every check returns NULL when the file is fine, or a message in the
 * caller's buffer naming the rule broken. RFC section numbers in comments
 * are those of RFC 8536.
 */
#ifndef TZIF_H
#define TZIF_H

#include <dirent.h>
#include <stdint.h>
#include <time.h>

struct tzh {			/* a header (3.1) */
	unsigned char ver;
	uint32_t isut, isstd, leap, tim, typ, chr;
};

struct tzb {			/* a parsed data block (3.2) */
	struct tzh h;
	const unsigned char *p;	/* start of the block's data */
	size_t len;		/* length of the block's data */
	int tsize;		/* 4 or 8 */
};

struct tzf {
	unsigned char *buf;
	size_t len;
	unsigned char ver;	/* 0, '2', '3' */
	struct tzb b1, b2;	/* b2 valid when ver != 0 */
	const unsigned char *foot;	/* footer (3.3), without its newlines */
	size_t footlen;
};

static uint32_t tz_u32(const unsigned char *p)
{
	return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) | p[3];
}

static int32_t tz_s32(const unsigned char *p)
{
	return (int32_t)tz_u32(p);
}

static int64_t tz_s64(const unsigned char *p)
{
	return (int64_t)(((uint64_t)tz_u32(p) << 32) | tz_u32(p + 4));
}

static int64_t tz_time(const unsigned char *p, int tsize)
{
	return tsize == 4 ? (int64_t)tz_s32(p) : tz_s64(p);
}

/* element offsets inside a data block's data */
static size_t tz_o_types(const struct tzb *b) { return (size_t)b->h.tim * b->tsize; }
static size_t tz_o_ttinfo(const struct tzb *b) { return tz_o_types(b) + b->h.tim; }
static size_t tz_o_chars(const struct tzb *b) { return tz_o_ttinfo(b) + (size_t)b->h.typ * 6; }
static size_t tz_o_leap(const struct tzb *b) { return tz_o_chars(b) + b->h.chr; }
static size_t tz_o_isstd(const struct tzb *b) { return tz_o_leap(b) + (size_t)b->h.leap * (b->tsize + 4); }
static size_t tz_o_isut(const struct tzb *b) { return tz_o_isstd(b) + b->h.isstd; }

static size_t tz_datalen(const struct tzh *h, int tsize)
{
	return (size_t)h->tim * tsize + h->tim + (size_t)h->typ * 6 + h->chr +
	    (size_t)h->leap * (tsize + 4) + h->isstd + h->isut;
}

/* Read a header at off; message on failure (3.1). */
static const char *tz_header(const unsigned char *buf, size_t len, size_t off, struct tzh *h)
{
	const unsigned char *p = buf + off;

	if (len < off + 44)
		return "file ends inside a header";
	if (memcmp(p, "TZif", 4) != 0)
		return "magic is not TZif";
	h->ver = p[4];
	h->isut = tz_u32(p + 20);
	h->isstd = tz_u32(p + 24);
	h->leap = tz_u32(p + 28);
	h->tim = tz_u32(p + 32);
	h->typ = tz_u32(p + 36);
	h->chr = tz_u32(p + 40);
	/* a count so large that the block cannot fit also gets rejected below */
	if (h->tim > 0x1000000 || h->typ > 0x1000000 || h->chr > 0x1000000 || h->leap > 0x1000000)
		return "a count in the header is absurdly large";
	if (h->typ == 0)
		return "typecnt is zero";
	if (h->chr == 0)
		return "charcnt is zero";
	if (h->isut != 0 && h->isut != h->typ)
		return "isutcnt is neither zero nor typecnt";
	if (h->isstd != 0 && h->isstd != h->typ)
		return "isstdcnt is neither zero nor typecnt";
	return NULL;
}

/*
 * Parse the layout: header, version 1 block, and for versions 2 and 3 the
 * second header, block and footer (3). Returns NULL, or a message. *skip is
 * set for a version byte this RFC does not define (the file is then not
 * judged).
 */
static const char *tz_layout(struct tzf *f, int *skip)
{
	const char *e;
	size_t off, blk;
	unsigned i;

	*skip = 0;
	e = tz_header(f->buf, f->len, 0, &f->b1.h);
	if (e != NULL)
		return e;
	f->ver = f->b1.h.ver;
	if (f->ver != 0 && f->ver != '2' && f->ver != '3') {
		if (f->ver > '3' && f->ver <= '9') {
			*skip = 1;
			return "version defined after RFC 8536";
		}
		return "version byte is not NUL, '2' or '3'";
	}
	f->b1.tsize = 4;
	f->b1.len = tz_datalen(&f->b1.h, 4);
	if (f->len < 44 + f->b1.len)
		return "file ends inside the version 1 data block";
	f->b1.p = f->buf + 44;
	off = 44 + f->b1.len;
	if (f->ver == 0) {
		if (f->len != off)
			return "version 1 file has bytes after the version 1 data block";
		return NULL;
	}
	e = tz_header(f->buf, f->len, off, &f->b2.h);
	if (e != NULL)
		return e;
	if (f->b2.h.ver != f->ver)
		return "the version 2+ header has another version byte than the first";
	f->b2.tsize = 8;
	blk = tz_datalen(&f->b2.h, 8);
	if (f->len < off + 44 + blk)
		return "file ends inside the version 2+ data block";
	f->b2.len = blk;
	f->b2.p = f->buf + off + 44;
	off += 44 + blk;
	if (f->len < off + 2)
		return "no footer after the version 2+ data block";
	if (f->buf[off] != '\n')
		return "footer does not begin with a newline";
	if (f->buf[f->len - 1] != '\n')
		return "footer does not end with a newline";
	f->foot = f->buf + off + 1;
	f->footlen = f->len - off - 2;
	for (i = 0; i < f->footlen; i++) {
		if (f->foot[i] == '\n')
			return "footer has a newline inside the TZ string or bytes follow it";
		if (f->foot[i] == '\0')
			return "footer TZ string contains a NUL";
	}
	return NULL;
}

/* The MUSTs about the content of one data block (3.2). */
static const char *tz_data(const struct tzb *b, char *msg, size_t n)
{
	unsigned i;
	const unsigned char *t = b->p;
	const unsigned char *ty = b->p + tz_o_types(b);
	const unsigned char *tt = b->p + tz_o_ttinfo(b);
	const unsigned char *ch = b->p + tz_o_chars(b);
	const unsigned char *lp = b->p + tz_o_leap(b);
	const unsigned char *st = b->p + tz_o_isstd(b);
	const unsigned char *ut = b->p + tz_o_isut(b);
	int64_t prev = 0;
	int32_t pc = 0;

	for (i = 0; i < b->h.tim; i++) {
		int64_t v = tz_time(t + (size_t)i * b->tsize, b->tsize);

		if (i > 0 && v <= prev) {
			(void)snprintf(msg, n, "transition %u (%lld) is not after transition %u (%lld)", i, (long long)v, i - 1, (long long)prev);
			return msg;
		}
		prev = v;
		if (ty[i] >= b->h.typ) {
			(void)snprintf(msg, n, "transition %u has type index %u, but typecnt is %u", i, ty[i], b->h.typ);
			return msg;
		}
	}
	for (i = 0; i < b->h.typ; i++) {
		const unsigned char *r = tt + (size_t)i * 6;
		unsigned idx = r[5], k;

		if (tz_s32(r) == INT32_MIN) {
			(void)snprintf(msg, n, "type %u has utoff -2^31", i);
			return msg;
		}
		if (r[4] > 1) {
			(void)snprintf(msg, n, "type %u has isdst %u", i, r[4]);
			return msg;
		}
		if (idx >= b->h.chr) {
			(void)snprintf(msg, n, "type %u has designation index %u, but charcnt is %u", i, idx, b->h.chr);
			return msg;
		}
		for (k = idx; k < b->h.chr && ch[k] != '\0'; k++)
			;
		if (k == b->h.chr) {
			(void)snprintf(msg, n, "the designation of type %u is not NUL-terminated", i);
			return msg;
		}
	}
	for (i = 0; i < b->h.leap; i++) {
		const unsigned char *r = lp + (size_t)i * (b->tsize + 4);
		int64_t occ = tz_time(r, b->tsize);
		int32_t corr = tz_s32(r + b->tsize);

		if (i == 0) {
			if (occ < 0 || (corr != 1 && corr != -1)) {
				(void)snprintf(msg, n, "first leap-second record: occurrence %lld, correction %ld", (long long)occ, (long)corr);
				return msg;
			}
		} else {
			if (occ < prev + 2419199 || (corr != pc + 1 && corr != pc - 1)) {
				(void)snprintf(msg, n, "leap-second record %u: occurrence %lld after %lld, correction %ld after %ld", i, (long long)occ, (long long)prev, (long)corr, (long)pc);
				return msg;
			}
		}
		prev = occ;
		pc = corr;
	}
	for (i = 0; i < b->h.isstd; i++)
		if (st[i] > 1) {
			(void)snprintf(msg, n, "standard/wall indicator %u is %u", i, st[i]);
			return msg;
		}
	for (i = 0; i < b->h.isut; i++) {
		if (ut[i] > 1) {
			(void)snprintf(msg, n, "UT/local indicator %u is %u", i, ut[i]);
			return msg;
		}
		if (ut[i] == 1 && (b->h.isstd == 0 || st[i] != 1)) {
			(void)snprintf(msg, n, "UT/local indicator %u is set but the standard/wall indicator is not", i);
			return msg;
		}
	}
	return NULL;
}

/* Accessors for a block's content. */
static int64_t tzb_trans(const struct tzb *b, unsigned i)
{
	return tz_time(b->p + (size_t)i * b->tsize, b->tsize);
}
static unsigned tzb_ttype(const struct tzb *b, unsigned i)
{
	return b->p[tz_o_types(b) + i];
}
static int32_t tzb_utoff(const struct tzb *b, unsigned ty)
{
	return tz_s32(b->p + tz_o_ttinfo(b) + (size_t)ty * 6);
}
static int tzb_isdst(const struct tzb *b, unsigned ty)
{
	return b->p[tz_o_ttinfo(b) + (size_t)ty * 6 + 4];
}
static const char *tzb_abbr(const struct tzb *b, unsigned ty)
{
	return (const char *)b->p + tz_o_chars(b) + b->p[tz_o_ttinfo(b) + (size_t)ty * 6 + 5];
}

/*
 * Offset from UT of a broken-down local time, computed without any library
 * extension: local seconds since the Epoch minus the instant.
 */
static long long tz_local_offset(const struct tm *tm, time_t t)
{
	long long y = (long long)tm->tm_year + 1900, m = tm->tm_mon + 1, d = tm->tm_mday, era, yoe, doy, doe, days;

	y -= m <= 2;
	era = (y >= 0 ? y : y - 399) / 400;
	yoe = y - era * 400;
	doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
	doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
	days = era * 146097 + doe - 719468;
	return days * 86400 + tm->tm_hour * 3600LL + tm->tm_min * 60LL + tm->tm_sec - (long long)t;
}

/* Does the zone database directory d hold a TZif file called UTC? */
static int tz_has_utc(const char *d)
{
	char p[1024];
	unsigned char m[4];
	int fd;
	ssize_t r;

	(void)snprintf(p, sizeof p, "%s/UTC", d);
	fd = open(p, O_RDONLY);
	if (fd < 0)
		return 0;
	r = read(fd, m, 4);
	(void)close(fd);
	return r == 4 && memcmp(m, "TZif", 4) == 0;
}

/* The zone database directory: PC_CONV_TZDIR (the caller's TZDIR), else a
 * common location; NULL when none holds a UTC file. */
static const char *tz_find_dir(void)
{
	static const char *const common[] = {
		"/usr/share/zoneinfo", "/usr/share/lib/zoneinfo", "/usr/lib/zoneinfo",
		"/usr/local/etc/zoneinfo", "/usr/local/share/zoneinfo", "/etc/zoneinfo", NULL
	};
	const char *e = cv_env("TZDIR");
	int i;

	if (e != NULL)
		return tz_has_utc(e) ? e : NULL;
	for (i = 0; common[i] != NULL; i++)
		if (tz_has_utc(common[i]))
			return common[i];
	return NULL;
}

/* Call fn(path, rel) for every regular file below dir that begins with the
 * TZif magic; at most `max` files. */
static int tz_walk(const char *dir, const char *rel, int depth, int max, int *count,
    void (*fn)(const char *path, const char *rel))
{
	DIR *dp = opendir(dir);
	struct dirent *e;
	char path[1024], r2[512];
	struct stat st, lst;
	unsigned char m[4];
	int fd;

	if (dp == NULL)
		return 0;
	while ((e = readdir(dp)) != NULL && *count < max) {
		if (e->d_name[0] == '.')
			continue;
		(void)snprintf(path, sizeof path, "%s/%s", dir, e->d_name);
		(void)snprintf(r2, sizeof r2, "%s%s%s", rel, rel[0] != '\0' ? "/" : "", e->d_name);
		if (lstat(path, &lst) != 0 || stat(path, &st) != 0)
			continue;
		if (S_ISDIR(st.st_mode)) {
			if (!S_ISLNK(lst.st_mode) && depth < 6)
				(void)tz_walk(path, r2, depth + 1, max, count, fn);
			continue;
		}
		if (!S_ISREG(st.st_mode))
			continue;
		fd = open(path, O_RDONLY);
		if (fd < 0)
			continue;
		if (read(fd, m, 4) == 4 && memcmp(m, "TZif", 4) == 0) {
			(*count)++;
			(void)close(fd);
			fn(path, r2);
			continue;
		}
		(void)close(fd);
	}
	(void)closedir(dp);
	return *count;
}

#endif
