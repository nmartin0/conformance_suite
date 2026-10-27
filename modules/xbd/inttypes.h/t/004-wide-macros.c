/*
 * The PRI and SCN macros in wide formats.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.inttypes.h.004
 */
#include <inttypes.h>
#include <wchar.h>
#include <stdio.h>
#include "pc.h"

#define WPRT(fm, jf, name, T, v) do { \
	wchar_t g[80], w[80]; \
	swprintf(g, 80, L"%" fm, (T)(v)); \
	swprintf(w, 80, jf, (uintmax_t)(T)(v)); \
	pc_check(wcscmp(g, w) == 0, "%s printed a different wide text from the j length", name); \
} while (0)
#define WPRT_S(fm, name, T, v) do { \
	wchar_t g[80], w[80]; \
	swprintf(g, 80, L"%" fm, (T)(v)); \
	swprintf(w, 80, L"%jd", (intmax_t)(T)(v)); \
	pc_check(wcscmp(g, w) == 0, "%s printed a different wide text from the j length", name); \
} while (0)
#define WSCN(fm, name, T, v) do { \
	wchar_t t[80]; \
	T x = 0; \
	int n; \
	swprintf(t, 80, L"%ju", (uintmax_t)(T)(v)); \
	n = swscanf(t, L"%" fm, &x); \
	pc_check(n == 1 && x == (T)(v), "%s read a wide text as a different value (%d items)", name, n); \
} while (0)
#define WSCN_S(fm, name, T, v) do { \
	wchar_t t[80]; \
	T x = 0; \
	int n; \
	swprintf(t, 80, L"%jd", (intmax_t)(T)(v)); \
	n = swscanf(t, L"%" fm, &x); \
	pc_check(n == 1 && x == (T)(v), "%s read a wide text as a different value (%d items)", name, n); \
} while (0)

int main(void)
{
	pc_begin("XBD.inttypes.h.004");
	pc_test();
	WPRT_S(PRId8, "PRId8", int8_t, INT8_MIN); WPRT_S(PRId8, "PRId8", int8_t, INT8_MAX); WPRT_S(PRIi8, "PRIi8", int8_t, -1);
	WPRT(PRIu8, L"%ju", "PRIu8", uint8_t, UINT8_MAX); WPRT(PRIx8, L"%jx", "PRIx8", uint8_t, UINT8_MAX); WPRT(PRIX8, L"%jX", "PRIX8", uint8_t, UINT8_MAX); WPRT(PRIo8, L"%jo", "PRIo8", uint8_t, UINT8_MAX);
#ifdef SCNd8
	WSCN_S(SCNd8, "SCNd8", int8_t, INT8_MAX); WSCN_S(SCNd8, "SCNd8", int8_t, -1);
#endif
#ifdef SCNu8
	WSCN(SCNu8, "SCNu8", uint8_t, UINT8_MAX);
#endif
	WPRT_S(PRId16, "PRId16", int16_t, INT16_MIN); WPRT_S(PRId16, "PRId16", int16_t, INT16_MAX); WPRT_S(PRIi16, "PRIi16", int16_t, -1);
	WPRT(PRIu16, L"%ju", "PRIu16", uint16_t, UINT16_MAX); WPRT(PRIx16, L"%jx", "PRIx16", uint16_t, UINT16_MAX); WPRT(PRIX16, L"%jX", "PRIX16", uint16_t, UINT16_MAX); WPRT(PRIo16, L"%jo", "PRIo16", uint16_t, UINT16_MAX);
#ifdef SCNd16
	WSCN_S(SCNd16, "SCNd16", int16_t, INT16_MAX); WSCN_S(SCNd16, "SCNd16", int16_t, -1);
#endif
#ifdef SCNu16
	WSCN(SCNu16, "SCNu16", uint16_t, UINT16_MAX);
#endif
	WPRT_S(PRId32, "PRId32", int32_t, INT32_MIN); WPRT_S(PRId32, "PRId32", int32_t, INT32_MAX); WPRT_S(PRIi32, "PRIi32", int32_t, -1);
	WPRT(PRIu32, L"%ju", "PRIu32", uint32_t, UINT32_MAX); WPRT(PRIx32, L"%jx", "PRIx32", uint32_t, UINT32_MAX); WPRT(PRIX32, L"%jX", "PRIX32", uint32_t, UINT32_MAX); WPRT(PRIo32, L"%jo", "PRIo32", uint32_t, UINT32_MAX);
#ifdef SCNd32
	WSCN_S(SCNd32, "SCNd32", int32_t, INT32_MAX); WSCN_S(SCNd32, "SCNd32", int32_t, -1);
#endif
#ifdef SCNu32
	WSCN(SCNu32, "SCNu32", uint32_t, UINT32_MAX);
#endif
	WPRT_S(PRIdLEAST16, "PRIdLEAST16", int_least16_t, INT_LEAST16_MIN); WPRT_S(PRIdLEAST16, "PRIdLEAST16", int_least16_t, INT_LEAST16_MAX); WPRT_S(PRIiLEAST16, "PRIiLEAST16", int_least16_t, -1);
	WPRT(PRIuLEAST16, L"%ju", "PRIuLEAST16", uint_least16_t, UINT_LEAST16_MAX); WPRT(PRIxLEAST16, L"%jx", "PRIxLEAST16", uint_least16_t, UINT_LEAST16_MAX); WPRT(PRIXLEAST16, L"%jX", "PRIXLEAST16", uint_least16_t, UINT_LEAST16_MAX); WPRT(PRIoLEAST16, L"%jo", "PRIoLEAST16", uint_least16_t, UINT_LEAST16_MAX);
#ifdef SCNdLEAST16
	WSCN_S(SCNdLEAST16, "SCNdLEAST16", int_least16_t, INT_LEAST16_MAX); WSCN_S(SCNdLEAST16, "SCNdLEAST16", int_least16_t, -1);
#endif
#ifdef SCNuLEAST16
	WSCN(SCNuLEAST16, "SCNuLEAST16", uint_least16_t, UINT_LEAST16_MAX);
#endif
	WPRT_S(PRIdFAST32, "PRIdFAST32", int_fast32_t, INT_FAST32_MIN); WPRT_S(PRIdFAST32, "PRIdFAST32", int_fast32_t, INT_FAST32_MAX); WPRT_S(PRIiFAST32, "PRIiFAST32", int_fast32_t, -1);
	WPRT(PRIuFAST32, L"%ju", "PRIuFAST32", uint_fast32_t, UINT_FAST32_MAX); WPRT(PRIxFAST32, L"%jx", "PRIxFAST32", uint_fast32_t, UINT_FAST32_MAX); WPRT(PRIXFAST32, L"%jX", "PRIXFAST32", uint_fast32_t, UINT_FAST32_MAX); WPRT(PRIoFAST32, L"%jo", "PRIoFAST32", uint_fast32_t, UINT_FAST32_MAX);
#ifdef SCNdFAST32
	WSCN_S(SCNdFAST32, "SCNdFAST32", int_fast32_t, INT_FAST32_MAX); WSCN_S(SCNdFAST32, "SCNdFAST32", int_fast32_t, -1);
#endif
#ifdef SCNuFAST32
	WSCN(SCNuFAST32, "SCNuFAST32", uint_fast32_t, UINT_FAST32_MAX);
#endif
	WPRT_S(PRIdMAX, "PRIdMAX", intmax_t, INTMAX_MIN); WPRT_S(PRIdMAX, "PRIdMAX", intmax_t, INTMAX_MAX); WPRT_S(PRIiMAX, "PRIiMAX", intmax_t, -1);
	WPRT(PRIuMAX, L"%ju", "PRIuMAX", uintmax_t, UINTMAX_MAX); WPRT(PRIxMAX, L"%jx", "PRIxMAX", uintmax_t, UINTMAX_MAX); WPRT(PRIXMAX, L"%jX", "PRIXMAX", uintmax_t, UINTMAX_MAX); WPRT(PRIoMAX, L"%jo", "PRIoMAX", uintmax_t, UINTMAX_MAX);
#ifdef SCNdMAX
	WSCN_S(SCNdMAX, "SCNdMAX", intmax_t, INTMAX_MAX); WSCN_S(SCNdMAX, "SCNdMAX", intmax_t, -1);
#endif
#ifdef SCNuMAX
	WSCN(SCNuMAX, "SCNuMAX", uintmax_t, UINTMAX_MAX);
#endif
	pc_pass();
}
