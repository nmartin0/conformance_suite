/*
 * The PRI macros print correct text through snprintf.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.inttypes.h.002
 */
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "pc.h"

static void chk(const char *what, const char *got, const char *want)
{
	pc_check(strcmp(got, want) == 0, "%s printed \"%s\", expected \"%s\"", what, got, want);
}

/* One value, one macro: print with the PRI macro and with the j length, and compare. */
#define PRT_S(fm, name, T, v) do { \
	char g[80], w[80]; \
	snprintf(g, sizeof g, "%" fm, (T)(v)); \
	snprintf(w, sizeof w, "%jd", (intmax_t)(T)(v)); \
	chk(name, g, w); \
} while (0)
#define PRT_U(fm, jf, name, T, v) do { \
	char g[80], w[80]; \
	snprintf(g, sizeof g, "%" fm, (T)(v)); \
	snprintf(w, sizeof w, jf, (uintmax_t)(T)(v)); \
	chk(name, g, w); \
} while (0)
#define SGRP(sfx, T, mn, mx) do { \
	static const char nd[] = "PRId" #sfx, ni[] = "PRIi" #sfx; \
	PRT_S(PRId##sfx, nd, T, mn); PRT_S(PRId##sfx, nd, T, mx); PRT_S(PRId##sfx, nd, T, 0); PRT_S(PRId##sfx, nd, T, -1); PRT_S(PRId##sfx, nd, T, 1); \
	PRT_S(PRIi##sfx, ni, T, mn); PRT_S(PRIi##sfx, ni, T, mx); PRT_S(PRIi##sfx, ni, T, 0); PRT_S(PRIi##sfx, ni, T, -1); PRT_S(PRIi##sfx, ni, T, 1); \
} while (0)
#define UGRP1(fm, jf, name, T, um) do { \
	PRT_U(fm, jf, name, T, um); PRT_U(fm, jf, name, T, 0); PRT_U(fm, jf, name, T, 1); PRT_U(fm, jf, name, T, (um) / 2); PRT_U(fm, jf, name, T, 100); \
} while (0)
#define UGRP(sfx, T, um) do { \
	UGRP1(PRIo##sfx, "%jo", "PRIo" #sfx, T, um); UGRP1(PRIu##sfx, "%ju", "PRIu" #sfx, T, um); \
	UGRP1(PRIx##sfx, "%jx", "PRIx" #sfx, T, um); UGRP1(PRIX##sfx, "%jX", "PRIX" #sfx, T, um); \
} while (0)

int main(void)
{
	pc_begin("XBD.inttypes.h.002");
	pc_test();
#ifdef INT8_MAX
	SGRP(8, int8_t, INT8_MIN, INT8_MAX);
	UGRP(8, uint8_t, UINT8_MAX);
#endif
#ifdef INT16_MAX
	SGRP(16, int16_t, INT16_MIN, INT16_MAX);
	UGRP(16, uint16_t, UINT16_MAX);
#endif
#ifdef INT32_MAX
	SGRP(32, int32_t, INT32_MIN, INT32_MAX);
	UGRP(32, uint32_t, UINT32_MAX);
#endif
#ifdef INT64_MAX
	SGRP(64, int64_t, INT64_MIN, INT64_MAX);
	UGRP(64, uint64_t, UINT64_MAX);
#endif
#ifdef INT_LEAST8_MAX
	SGRP(LEAST8, int_least8_t, INT_LEAST8_MIN, INT_LEAST8_MAX);
	UGRP(LEAST8, uint_least8_t, UINT_LEAST8_MAX);
#endif
#ifdef INT_LEAST16_MAX
	SGRP(LEAST16, int_least16_t, INT_LEAST16_MIN, INT_LEAST16_MAX);
	UGRP(LEAST16, uint_least16_t, UINT_LEAST16_MAX);
#endif
#ifdef INT_LEAST32_MAX
	SGRP(LEAST32, int_least32_t, INT_LEAST32_MIN, INT_LEAST32_MAX);
	UGRP(LEAST32, uint_least32_t, UINT_LEAST32_MAX);
#endif
#ifdef INT_LEAST64_MAX
	SGRP(LEAST64, int_least64_t, INT_LEAST64_MIN, INT_LEAST64_MAX);
	UGRP(LEAST64, uint_least64_t, UINT_LEAST64_MAX);
#endif
#ifdef INT_FAST8_MAX
	SGRP(FAST8, int_fast8_t, INT_FAST8_MIN, INT_FAST8_MAX);
	UGRP(FAST8, uint_fast8_t, UINT_FAST8_MAX);
#endif
#ifdef INT_FAST16_MAX
	SGRP(FAST16, int_fast16_t, INT_FAST16_MIN, INT_FAST16_MAX);
	UGRP(FAST16, uint_fast16_t, UINT_FAST16_MAX);
#endif
#ifdef INT_FAST32_MAX
	SGRP(FAST32, int_fast32_t, INT_FAST32_MIN, INT_FAST32_MAX);
	UGRP(FAST32, uint_fast32_t, UINT_FAST32_MAX);
#endif
#ifdef INT_FAST64_MAX
	SGRP(FAST64, int_fast64_t, INT_FAST64_MIN, INT_FAST64_MAX);
	UGRP(FAST64, uint_fast64_t, UINT_FAST64_MAX);
#endif
#ifdef INTMAX_MAX
	SGRP(MAX, intmax_t, INTMAX_MIN, INTMAX_MAX);
	UGRP(MAX, uintmax_t, UINTMAX_MAX);
#endif
#ifdef INTPTR_MAX
	SGRP(PTR, intptr_t, INTPTR_MIN, INTPTR_MAX);
	UGRP(PTR, uintptr_t, UINTPTR_MAX);
#endif
	pc_pass();
}
