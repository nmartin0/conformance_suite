/*
 * elfdump.c - a small reader for object files in the common ELF container.
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * Written from the generic object file specification (header, section
 * headers, symbol tables, dynamic section), with its own field layout, so
 * that the toolchain tests do not depend on the host's reader program or
 * header. Handles both classes and both byte orders.
 *
 * usage: elfdump FILE
 *
 * Output, one record per line, fields separated by single blanks (a name
 * that is empty is written as "-"):
 *   ELF class=32|64 data=lsb|msb type=REL|EXEC|DYN|n machine=N entry=0xH
 *   SEC INDEX NAME TYPE FLAGS SIZE LINK INFO
 *   SYM TABLE INDEX NAME bind=B type=T shndx=S sec=NAME value=0xH size=N
 *   DYN TAG VALUE STRING
 *   PHD TYPE
 * Exit status 0 on success, 1 when FILE is not a well-formed object file.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *img;
static size_t len;
static int is64, msb;

static unsigned long long rd(size_t off, int n)
{
	unsigned long long v = 0;
	int i;

	if (off > len || (size_t)n > len - off) {
		(void)fprintf(stderr, "elfdump: truncated\n");
		exit(1);
	}
	for (i = 0; i < n; i++)
		v = (v << 8) | img[off + (msb ? i : n - 1 - i)];
	return v;
}

static const char *str(size_t tab, size_t tablen, unsigned long long off)
{
	size_t i;

	if (off >= tablen || tab > len || tablen > len - tab)
		return NULL;
	for (i = (size_t)off; i < tablen; i++)
		if (img[tab + i] == '\0')
			return (const char *)img + tab + off;
	return NULL;
}

static const char *nm(const char *s)
{
	return (s == NULL || s[0] == '\0') ? "-" : s;
}

struct sec {
	unsigned long long type, flags, addr, off, size, link, info, entsize;
	const char *name;
};

int main(int argc, char **argv)
{
	FILE *f;
	size_t cap = 1 << 16, shoff, phoff;
	unsigned long long et, machine, entry, shnum, shstrndx, shentsize, phnum, phentsize;
	struct sec *s;
	size_t i, j, n;
	const char *tn;

	if (argc != 2) {
		(void)fprintf(stderr, "usage: elfdump FILE\n");
		return 2;
	}
	f = fopen(argv[1], "rb");
	if (f == NULL)
		return 1;
	img = malloc(cap);
	while (img != NULL) {
		n = fread(img + len, 1, cap - len, f);
		len += n;
		if (len < cap)
			break;
		cap *= 2;
		img = realloc(img, cap);
	}
	(void)fclose(f);
	if (img == NULL || len < 52 || memcmp(img, "\177ELF", 4) != 0) {
		(void)fprintf(stderr, "elfdump: not an object file\n");
		return 1;
	}
	is64 = img[4] == 2;
	msb = img[5] == 2;
	if ((img[4] != 1 && img[4] != 2) || (img[5] != 1 && img[5] != 2) || (is64 && len < 64)) {
		(void)fprintf(stderr, "elfdump: unknown class or byte order\n");
		return 1;
	}
	et = rd(16, 2);
	machine = rd(18, 2);
	if (is64) {
		entry = rd(24, 8); phoff = (size_t)rd(32, 8); shoff = (size_t)rd(40, 8);
		phentsize = rd(54, 2); phnum = rd(56, 2); shentsize = rd(58, 2); shnum = rd(60, 2); shstrndx = rd(62, 2);
	} else {
		entry = rd(24, 4); phoff = (size_t)rd(28, 4); shoff = (size_t)rd(32, 4);
		phentsize = rd(42, 2); phnum = rd(44, 2); shentsize = rd(46, 2); shnum = rd(48, 2); shstrndx = rd(50, 2);
	}
	tn = et == 1 ? "REL" : et == 2 ? "EXEC" : et == 3 ? "DYN" : et == 4 ? "CORE" : "n";
	(void)printf("ELF class=%d data=%s type=%s machine=%llu entry=0x%llx\n", is64 ? 64 : 32, msb ? "msb" : "lsb", tn, machine, entry);
	for (i = 0; i < phnum && phentsize >= 8; i++)
		(void)printf("PHD %llu\n", rd(phoff + i * phentsize, 4));
	if (shnum == 0 || shentsize < 40) {
		return 0;
	}
	s = calloc((size_t)shnum, sizeof *s);
	if (s == NULL)
		return 1;
	for (i = 0; i < shnum; i++) {
		size_t b = shoff + i * shentsize;

		if (is64) {
			s[i].type = rd(b + 4, 4); s[i].flags = rd(b + 8, 8); s[i].addr = rd(b + 16, 8);
			s[i].off = rd(b + 24, 8); s[i].size = rd(b + 32, 8); s[i].link = rd(b + 40, 4);
			s[i].info = rd(b + 44, 4); s[i].entsize = rd(b + 56, 8);
		} else {
			s[i].type = rd(b + 4, 4); s[i].flags = rd(b + 8, 4); s[i].addr = rd(b + 12, 4);
			s[i].off = rd(b + 16, 4); s[i].size = rd(b + 20, 4); s[i].link = rd(b + 24, 4);
			s[i].info = rd(b + 28, 4); s[i].entsize = rd(b + 36, 4);
		}
	}
	if (shstrndx < shnum) {
		struct sec *ss = &s[shstrndx];

		for (i = 0; i < shnum; i++) {
			size_t b = shoff + i * shentsize;

			s[i].name = str((size_t)ss->off, (size_t)ss->size, rd(b, 4));
		}
	}
	for (i = 0; i < shnum; i++) {
		static const char *const tnames[] = { "NULL", "PROGBITS", "SYMTAB", "STRTAB", "RELA", "HASH", "DYNAMIC",
		    "NOTE", "NOBITS", "REL", "SHLIB", "DYNSYM", "n12", "n13", "INIT_ARRAY", "FINI_ARRAY",
		    "PREINIT_ARRAY", "GROUP", "SYMTAB_SHNDX" };
		char tb[32], fl[16];
		int k = 0;

		if (s[i].type < sizeof tnames / sizeof tnames[0])
			(void)snprintf(tb, sizeof tb, "%s", tnames[s[i].type]);
		else
			(void)snprintf(tb, sizeof tb, "n%llu", s[i].type);
		if (s[i].flags & 1) fl[k++] = 'W';
		if (s[i].flags & 2) fl[k++] = 'A';
		if (s[i].flags & 4) fl[k++] = 'X';
		if (k == 0) fl[k++] = '-';
		fl[k] = '\0';
		(void)printf("SEC %zu %s %s %s %llu %llu %llu\n", i, nm(s[i].name), tb, fl, s[i].size, s[i].link, s[i].info);
	}
	for (i = 0; i < shnum; i++) {
		size_t esz, cnt, stab, stablen;

		if (s[i].type != 2 && s[i].type != 11)
			continue;
		esz = is64 ? 24 : 16;
		if (s[i].link >= shnum)
			continue;
		stab = (size_t)s[s[i].link].off;
		stablen = (size_t)s[s[i].link].size;
		cnt = (size_t)(s[i].size / esz);
		for (j = 0; j < cnt; j++) {
			size_t b = (size_t)s[i].off + j * esz;
			unsigned long long name, value, size, shndx;
			unsigned info, other;
			static const char *const bn[] = { "LOCAL", "GLOBAL", "WEAK" };
			static const char *const yn[] = { "NOTYPE", "OBJECT", "FUNC", "SECTION", "FILE", "COMMON", "TLS" };
			char bb[16], yb[16], sb[24];
			const char *secn = "-";

			if (is64) {
				name = rd(b, 4); info = (unsigned)rd(b + 4, 1); other = (unsigned)rd(b + 5, 1);
				shndx = rd(b + 6, 2); value = rd(b + 8, 8); size = rd(b + 16, 8);
			} else {
				name = rd(b, 4); value = rd(b + 4, 4); size = rd(b + 8, 4);
				info = (unsigned)rd(b + 12, 1); other = (unsigned)rd(b + 13, 1); shndx = rd(b + 14, 2);
			}
			if ((info >> 4) < 3) (void)snprintf(bb, sizeof bb, "%s", bn[info >> 4]); else (void)snprintf(bb, sizeof bb, "n%u", info >> 4);
			if ((info & 15) < 7) (void)snprintf(yb, sizeof yb, "%s", yn[info & 15]); else (void)snprintf(yb, sizeof yb, "n%u", info & 15);
			if (shndx == 0) (void)snprintf(sb, sizeof sb, "UND");
			else if (shndx == 0xfff1) (void)snprintf(sb, sizeof sb, "ABS");
			else if (shndx == 0xfff2) (void)snprintf(sb, sizeof sb, "COM");
			else (void)snprintf(sb, sizeof sb, "%llu", shndx);
			if (shndx > 0 && shndx < shnum && s[shndx].name != NULL)
				secn = nm(s[shndx].name);
			(void)printf("SYM %s %zu %s bind=%s type=%s shndx=%s sec=%s value=0x%llx size=%llu other=%u\n",
			    nm(s[i].name), j, nm(str(stab, stablen, name)), bb, yb, sb, secn, value, size, other & 3);
		}
	}
	for (i = 0; i < shnum; i++) {
		size_t esz, cnt, stab, stablen;

		if (s[i].type != 6 || s[i].link >= shnum)
			continue;
		esz = is64 ? 16 : 8;
		stab = (size_t)s[s[i].link].off;
		stablen = (size_t)s[s[i].link].size;
		cnt = (size_t)(s[i].size / esz);
		for (j = 0; j < cnt; j++) {
			size_t b = (size_t)s[i].off + j * esz;
			unsigned long long tag = rd(b, is64 ? 8 : 4), val = rd(b + (is64 ? 8 : 4), is64 ? 8 : 4);
			const char *tg = NULL, *sv = "-";
			char tb[24];

			switch (tag) {
			case 0: tg = "NULL"; break;
			case 1: tg = "NEEDED"; sv = nm(str(stab, stablen, val)); break;
			case 12: tg = "INIT"; break;
			case 13: tg = "FINI"; break;
			case 14: tg = "SONAME"; sv = nm(str(stab, stablen, val)); break;
			case 15: tg = "RPATH"; sv = nm(str(stab, stablen, val)); break;
			case 25: tg = "INIT_ARRAY"; break;
			case 26: tg = "FINI_ARRAY"; break;
			case 27: tg = "INIT_ARRAYSZ"; break;
			case 28: tg = "FINI_ARRAYSZ"; break;
			case 29: tg = "RUNPATH"; sv = nm(str(stab, stablen, val)); break;
			case 30: tg = "FLAGS"; break;
			default:
				(void)snprintf(tb, sizeof tb, "n%llu", tag);
				tg = tb;
			}
			(void)printf("DYN %s %llu %s\n", tg, val, sv);
			if (tag == 0)
				break;
		}
	}
	return 0;
}
