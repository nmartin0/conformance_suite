/*
 * DT_BLK, DT_CHR, DT_DIR, DT_FIFO, DT_LNK, DT_REG, DT_SOCK and DT_UNKNOWN
 * are usable in #if, have eight different values and fit in d_type (an
 * unsigned char).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.dirent.h.001
 */
#include "pc.h"
#include <dirent.h>

#ifdef DT_BLK
#if !((DT_BLK) || 1)
#error "DT_BLK is not usable in #if"
#endif
#endif
#ifdef DT_CHR
#if !((DT_CHR) || 1)
#error "DT_CHR is not usable in #if"
#endif
#endif
#ifdef DT_DIR
#if !((DT_DIR) || 1)
#error "DT_DIR is not usable in #if"
#endif
#endif
#ifdef DT_FIFO
#if !((DT_FIFO) || 1)
#error "DT_FIFO is not usable in #if"
#endif
#endif
#ifdef DT_LNK
#if !((DT_LNK) || 1)
#error "DT_LNK is not usable in #if"
#endif
#endif
#ifdef DT_REG
#if !((DT_REG) || 1)
#error "DT_REG is not usable in #if"
#endif
#endif
#ifdef DT_SOCK
#if !((DT_SOCK) || 1)
#error "DT_SOCK is not usable in #if"
#endif
#endif
#ifdef DT_UNKNOWN
#if !((DT_UNKNOWN) || 1)
#error "DT_UNKNOWN is not usable in #if"
#endif
#endif

int main(void)
{
	pc_begin("XBD.dirent.h.001");
#if !defined(DT_BLK) || !defined(DT_CHR) || !defined(DT_DIR) || !defined(DT_FIFO) || !defined(DT_LNK) || !defined(DT_REG) || !defined(DT_SOCK) || !defined(DT_UNKNOWN)
	pc_unresolved("a constant is missing (the header check reports it): DT_BLK DT_CHR DT_DIR DT_FIFO DT_LNK DT_REG DT_SOCK DT_UNKNOWN");
#else
	{
	struct { const char *n; long long v; } e[9];
	int n = 0, i, j;

	e[n].n = "DT_BLK"; e[n++].v = (long long)(DT_BLK);
	e[n].n = "DT_CHR"; e[n++].v = (long long)(DT_CHR);
	e[n].n = "DT_DIR"; e[n++].v = (long long)(DT_DIR);
	e[n].n = "DT_FIFO"; e[n++].v = (long long)(DT_FIFO);
	e[n].n = "DT_LNK"; e[n++].v = (long long)(DT_LNK);
	e[n].n = "DT_REG"; e[n++].v = (long long)(DT_REG);
	e[n].n = "DT_SOCK"; e[n++].v = (long long)(DT_SOCK);
	e[n].n = "DT_UNKNOWN"; e[n++].v = (long long)(DT_UNKNOWN);
	if (n < 2)
		pc_unresolved("fewer than 2 of the constants are defined");
	pc_test();
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			pc_check(e[i].v != e[j].v, "%s and %s are both %lld", e[i].n, e[j].n, e[i].v);
		}
	for (i = 0; i < n; i++)
		pc_check((long long)(unsigned char)e[i].v == e[i].v, "%s does not fit in d_type (an unsigned char)", e[i].n);
	pc_pass();
	}
#endif
}
