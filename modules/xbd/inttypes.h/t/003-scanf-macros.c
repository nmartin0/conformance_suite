/*
 * The SCN macros read correct values through sscanf.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.inttypes.h.003
 */
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "pc.h"

#define SCN_S(fm, name, T, v) do { \
	char t[80]; \
	T x = 0; \
	int n; \
	snprintf(t, sizeof t, "%jd", (intmax_t)(T)(v)); \
	n = sscanf(t, "%" fm, &x); \
	pc_check(n == 1 && x == (T)(v), "%s read \"%s\" as a different value (%d items)", name, t, n); \
} while (0)
#define SCN_U(fm, jf, name, T, v) do { \
	char t[80]; \
	T x = 0; \
	int n; \
	snprintf(t, sizeof t, jf, (uintmax_t)(T)(v)); \
	n = sscanf(t, "%" fm, &x); \
	pc_check(n == 1 && x == (T)(v), "%s read \"%s\" as a different value (%d items)", name, t, n); \
} while (0)
#define SCN_SV(fm, name, T, mn, mx) do { SCN_S(fm, name, T, mn); SCN_S(fm, name, T, mx); SCN_S(fm, name, T, 0); SCN_S(fm, name, T, -1); SCN_S(fm, name, T, 1); } while (0)
#define SCN_UV(fm, jf, name, T, um) do { SCN_U(fm, jf, name, T, um); SCN_U(fm, jf, name, T, 0); SCN_U(fm, jf, name, T, 1); SCN_U(fm, jf, name, T, (um) / 2); SCN_U(fm, jf, name, T, 100); } while (0)

int main(void)
{
	pc_begin("XBD.inttypes.h.003");
	pc_test();
#ifdef INT8_MAX
#ifdef SCNd8
	SCN_SV(SCNd8, "SCNd8", int8_t, INT8_MIN, INT8_MAX);
#endif
#ifdef SCNi8
	SCN_SV(SCNi8, "SCNi8", int8_t, INT8_MIN, INT8_MAX);
	SCN_UV(SCNi8, "0x%jx", "SCNi8 (hex)", int8_t, INT8_MAX);
#endif
#ifdef SCNo8
	SCN_UV(SCNo8, "%jo", "SCNo8", uint8_t, UINT8_MAX);
#endif
#ifdef SCNu8
	SCN_UV(SCNu8, "%ju", "SCNu8", uint8_t, UINT8_MAX);
#endif
#ifdef SCNx8
	SCN_UV(SCNx8, "%jx", "SCNx8", uint8_t, UINT8_MAX);
#endif
#endif
#ifdef INT16_MAX
#ifdef SCNd16
	SCN_SV(SCNd16, "SCNd16", int16_t, INT16_MIN, INT16_MAX);
#endif
#ifdef SCNi16
	SCN_SV(SCNi16, "SCNi16", int16_t, INT16_MIN, INT16_MAX);
	SCN_UV(SCNi16, "0x%jx", "SCNi16 (hex)", int16_t, INT16_MAX);
#endif
#ifdef SCNo16
	SCN_UV(SCNo16, "%jo", "SCNo16", uint16_t, UINT16_MAX);
#endif
#ifdef SCNu16
	SCN_UV(SCNu16, "%ju", "SCNu16", uint16_t, UINT16_MAX);
#endif
#ifdef SCNx16
	SCN_UV(SCNx16, "%jx", "SCNx16", uint16_t, UINT16_MAX);
#endif
#endif
#ifdef INT32_MAX
#ifdef SCNd32
	SCN_SV(SCNd32, "SCNd32", int32_t, INT32_MIN, INT32_MAX);
#endif
#ifdef SCNi32
	SCN_SV(SCNi32, "SCNi32", int32_t, INT32_MIN, INT32_MAX);
	SCN_UV(SCNi32, "0x%jx", "SCNi32 (hex)", int32_t, INT32_MAX);
#endif
#ifdef SCNo32
	SCN_UV(SCNo32, "%jo", "SCNo32", uint32_t, UINT32_MAX);
#endif
#ifdef SCNu32
	SCN_UV(SCNu32, "%ju", "SCNu32", uint32_t, UINT32_MAX);
#endif
#ifdef SCNx32
	SCN_UV(SCNx32, "%jx", "SCNx32", uint32_t, UINT32_MAX);
#endif
#endif
#ifdef INT64_MAX
#ifdef SCNd64
	SCN_SV(SCNd64, "SCNd64", int64_t, INT64_MIN, INT64_MAX);
#endif
#ifdef SCNi64
	SCN_SV(SCNi64, "SCNi64", int64_t, INT64_MIN, INT64_MAX);
	SCN_UV(SCNi64, "0x%jx", "SCNi64 (hex)", int64_t, INT64_MAX);
#endif
#ifdef SCNo64
	SCN_UV(SCNo64, "%jo", "SCNo64", uint64_t, UINT64_MAX);
#endif
#ifdef SCNu64
	SCN_UV(SCNu64, "%ju", "SCNu64", uint64_t, UINT64_MAX);
#endif
#ifdef SCNx64
	SCN_UV(SCNx64, "%jx", "SCNx64", uint64_t, UINT64_MAX);
#endif
#endif
#ifdef INT_LEAST8_MAX
#ifdef SCNdLEAST8
	SCN_SV(SCNdLEAST8, "SCNdLEAST8", int_least8_t, INT_LEAST8_MIN, INT_LEAST8_MAX);
#endif
#ifdef SCNiLEAST8
	SCN_SV(SCNiLEAST8, "SCNiLEAST8", int_least8_t, INT_LEAST8_MIN, INT_LEAST8_MAX);
	SCN_UV(SCNiLEAST8, "0x%jx", "SCNiLEAST8 (hex)", int_least8_t, INT_LEAST8_MAX);
#endif
#ifdef SCNoLEAST8
	SCN_UV(SCNoLEAST8, "%jo", "SCNoLEAST8", uint_least8_t, UINT_LEAST8_MAX);
#endif
#ifdef SCNuLEAST8
	SCN_UV(SCNuLEAST8, "%ju", "SCNuLEAST8", uint_least8_t, UINT_LEAST8_MAX);
#endif
#ifdef SCNxLEAST8
	SCN_UV(SCNxLEAST8, "%jx", "SCNxLEAST8", uint_least8_t, UINT_LEAST8_MAX);
#endif
#endif
#ifdef INT_LEAST16_MAX
#ifdef SCNdLEAST16
	SCN_SV(SCNdLEAST16, "SCNdLEAST16", int_least16_t, INT_LEAST16_MIN, INT_LEAST16_MAX);
#endif
#ifdef SCNiLEAST16
	SCN_SV(SCNiLEAST16, "SCNiLEAST16", int_least16_t, INT_LEAST16_MIN, INT_LEAST16_MAX);
	SCN_UV(SCNiLEAST16, "0x%jx", "SCNiLEAST16 (hex)", int_least16_t, INT_LEAST16_MAX);
#endif
#ifdef SCNoLEAST16
	SCN_UV(SCNoLEAST16, "%jo", "SCNoLEAST16", uint_least16_t, UINT_LEAST16_MAX);
#endif
#ifdef SCNuLEAST16
	SCN_UV(SCNuLEAST16, "%ju", "SCNuLEAST16", uint_least16_t, UINT_LEAST16_MAX);
#endif
#ifdef SCNxLEAST16
	SCN_UV(SCNxLEAST16, "%jx", "SCNxLEAST16", uint_least16_t, UINT_LEAST16_MAX);
#endif
#endif
#ifdef INT_LEAST32_MAX
#ifdef SCNdLEAST32
	SCN_SV(SCNdLEAST32, "SCNdLEAST32", int_least32_t, INT_LEAST32_MIN, INT_LEAST32_MAX);
#endif
#ifdef SCNiLEAST32
	SCN_SV(SCNiLEAST32, "SCNiLEAST32", int_least32_t, INT_LEAST32_MIN, INT_LEAST32_MAX);
	SCN_UV(SCNiLEAST32, "0x%jx", "SCNiLEAST32 (hex)", int_least32_t, INT_LEAST32_MAX);
#endif
#ifdef SCNoLEAST32
	SCN_UV(SCNoLEAST32, "%jo", "SCNoLEAST32", uint_least32_t, UINT_LEAST32_MAX);
#endif
#ifdef SCNuLEAST32
	SCN_UV(SCNuLEAST32, "%ju", "SCNuLEAST32", uint_least32_t, UINT_LEAST32_MAX);
#endif
#ifdef SCNxLEAST32
	SCN_UV(SCNxLEAST32, "%jx", "SCNxLEAST32", uint_least32_t, UINT_LEAST32_MAX);
#endif
#endif
#ifdef INT_LEAST64_MAX
#ifdef SCNdLEAST64
	SCN_SV(SCNdLEAST64, "SCNdLEAST64", int_least64_t, INT_LEAST64_MIN, INT_LEAST64_MAX);
#endif
#ifdef SCNiLEAST64
	SCN_SV(SCNiLEAST64, "SCNiLEAST64", int_least64_t, INT_LEAST64_MIN, INT_LEAST64_MAX);
	SCN_UV(SCNiLEAST64, "0x%jx", "SCNiLEAST64 (hex)", int_least64_t, INT_LEAST64_MAX);
#endif
#ifdef SCNoLEAST64
	SCN_UV(SCNoLEAST64, "%jo", "SCNoLEAST64", uint_least64_t, UINT_LEAST64_MAX);
#endif
#ifdef SCNuLEAST64
	SCN_UV(SCNuLEAST64, "%ju", "SCNuLEAST64", uint_least64_t, UINT_LEAST64_MAX);
#endif
#ifdef SCNxLEAST64
	SCN_UV(SCNxLEAST64, "%jx", "SCNxLEAST64", uint_least64_t, UINT_LEAST64_MAX);
#endif
#endif
#ifdef INT_FAST8_MAX
#ifdef SCNdFAST8
	SCN_SV(SCNdFAST8, "SCNdFAST8", int_fast8_t, INT_FAST8_MIN, INT_FAST8_MAX);
#endif
#ifdef SCNiFAST8
	SCN_SV(SCNiFAST8, "SCNiFAST8", int_fast8_t, INT_FAST8_MIN, INT_FAST8_MAX);
	SCN_UV(SCNiFAST8, "0x%jx", "SCNiFAST8 (hex)", int_fast8_t, INT_FAST8_MAX);
#endif
#ifdef SCNoFAST8
	SCN_UV(SCNoFAST8, "%jo", "SCNoFAST8", uint_fast8_t, UINT_FAST8_MAX);
#endif
#ifdef SCNuFAST8
	SCN_UV(SCNuFAST8, "%ju", "SCNuFAST8", uint_fast8_t, UINT_FAST8_MAX);
#endif
#ifdef SCNxFAST8
	SCN_UV(SCNxFAST8, "%jx", "SCNxFAST8", uint_fast8_t, UINT_FAST8_MAX);
#endif
#endif
#ifdef INT_FAST16_MAX
#ifdef SCNdFAST16
	SCN_SV(SCNdFAST16, "SCNdFAST16", int_fast16_t, INT_FAST16_MIN, INT_FAST16_MAX);
#endif
#ifdef SCNiFAST16
	SCN_SV(SCNiFAST16, "SCNiFAST16", int_fast16_t, INT_FAST16_MIN, INT_FAST16_MAX);
	SCN_UV(SCNiFAST16, "0x%jx", "SCNiFAST16 (hex)", int_fast16_t, INT_FAST16_MAX);
#endif
#ifdef SCNoFAST16
	SCN_UV(SCNoFAST16, "%jo", "SCNoFAST16", uint_fast16_t, UINT_FAST16_MAX);
#endif
#ifdef SCNuFAST16
	SCN_UV(SCNuFAST16, "%ju", "SCNuFAST16", uint_fast16_t, UINT_FAST16_MAX);
#endif
#ifdef SCNxFAST16
	SCN_UV(SCNxFAST16, "%jx", "SCNxFAST16", uint_fast16_t, UINT_FAST16_MAX);
#endif
#endif
#ifdef INT_FAST32_MAX
#ifdef SCNdFAST32
	SCN_SV(SCNdFAST32, "SCNdFAST32", int_fast32_t, INT_FAST32_MIN, INT_FAST32_MAX);
#endif
#ifdef SCNiFAST32
	SCN_SV(SCNiFAST32, "SCNiFAST32", int_fast32_t, INT_FAST32_MIN, INT_FAST32_MAX);
	SCN_UV(SCNiFAST32, "0x%jx", "SCNiFAST32 (hex)", int_fast32_t, INT_FAST32_MAX);
#endif
#ifdef SCNoFAST32
	SCN_UV(SCNoFAST32, "%jo", "SCNoFAST32", uint_fast32_t, UINT_FAST32_MAX);
#endif
#ifdef SCNuFAST32
	SCN_UV(SCNuFAST32, "%ju", "SCNuFAST32", uint_fast32_t, UINT_FAST32_MAX);
#endif
#ifdef SCNxFAST32
	SCN_UV(SCNxFAST32, "%jx", "SCNxFAST32", uint_fast32_t, UINT_FAST32_MAX);
#endif
#endif
#ifdef INT_FAST64_MAX
#ifdef SCNdFAST64
	SCN_SV(SCNdFAST64, "SCNdFAST64", int_fast64_t, INT_FAST64_MIN, INT_FAST64_MAX);
#endif
#ifdef SCNiFAST64
	SCN_SV(SCNiFAST64, "SCNiFAST64", int_fast64_t, INT_FAST64_MIN, INT_FAST64_MAX);
	SCN_UV(SCNiFAST64, "0x%jx", "SCNiFAST64 (hex)", int_fast64_t, INT_FAST64_MAX);
#endif
#ifdef SCNoFAST64
	SCN_UV(SCNoFAST64, "%jo", "SCNoFAST64", uint_fast64_t, UINT_FAST64_MAX);
#endif
#ifdef SCNuFAST64
	SCN_UV(SCNuFAST64, "%ju", "SCNuFAST64", uint_fast64_t, UINT_FAST64_MAX);
#endif
#ifdef SCNxFAST64
	SCN_UV(SCNxFAST64, "%jx", "SCNxFAST64", uint_fast64_t, UINT_FAST64_MAX);
#endif
#endif
#ifdef INTMAX_MAX
#ifdef SCNdMAX
	SCN_SV(SCNdMAX, "SCNdMAX", intmax_t, INTMAX_MIN, INTMAX_MAX);
#endif
#ifdef SCNiMAX
	SCN_SV(SCNiMAX, "SCNiMAX", intmax_t, INTMAX_MIN, INTMAX_MAX);
	SCN_UV(SCNiMAX, "0x%jx", "SCNiMAX (hex)", intmax_t, INTMAX_MAX);
#endif
#ifdef SCNoMAX
	SCN_UV(SCNoMAX, "%jo", "SCNoMAX", uintmax_t, UINTMAX_MAX);
#endif
#ifdef SCNuMAX
	SCN_UV(SCNuMAX, "%ju", "SCNuMAX", uintmax_t, UINTMAX_MAX);
#endif
#ifdef SCNxMAX
	SCN_UV(SCNxMAX, "%jx", "SCNxMAX", uintmax_t, UINTMAX_MAX);
#endif
#endif
#ifdef INTPTR_MAX
#ifdef SCNdPTR
	SCN_SV(SCNdPTR, "SCNdPTR", intptr_t, INTPTR_MIN, INTPTR_MAX);
#endif
#ifdef SCNiPTR
	SCN_SV(SCNiPTR, "SCNiPTR", intptr_t, INTPTR_MIN, INTPTR_MAX);
	SCN_UV(SCNiPTR, "0x%jx", "SCNiPTR (hex)", intptr_t, INTPTR_MAX);
#endif
#ifdef SCNoPTR
	SCN_UV(SCNoPTR, "%jo", "SCNoPTR", uintptr_t, UINTPTR_MAX);
#endif
#ifdef SCNuPTR
	SCN_UV(SCNuPTR, "%ju", "SCNuPTR", uintptr_t, UINTPTR_MAX);
#endif
#ifdef SCNxPTR
	SCN_UV(SCNxPTR, "%jx", "SCNxPTR", uintptr_t, UINTPTR_MAX);
#endif
#endif
	pc_pass();
}
