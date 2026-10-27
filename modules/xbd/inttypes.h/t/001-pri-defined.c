/*
 * The PRI macros exist for every provided integer type.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.inttypes.h.001
 */
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "pc.h"

#ifndef UINTMAX_MAX
#error "<inttypes.h> did not make the <stdint.h> macros visible"
#endif
#ifdef INT8_MAX
#if !defined(PRId8) || !defined(PRIi8) || !defined(PRIo8) || !defined(PRIu8) || !defined(PRIx8) || !defined(PRIX8)
#error "a PRI macro for the 8 types is not defined although the type is provided"
#endif
#endif
#ifdef INT16_MAX
#if !defined(PRId16) || !defined(PRIi16) || !defined(PRIo16) || !defined(PRIu16) || !defined(PRIx16) || !defined(PRIX16)
#error "a PRI macro for the 16 types is not defined although the type is provided"
#endif
#endif
#ifdef INT32_MAX
#if !defined(PRId32) || !defined(PRIi32) || !defined(PRIo32) || !defined(PRIu32) || !defined(PRIx32) || !defined(PRIX32)
#error "a PRI macro for the 32 types is not defined although the type is provided"
#endif
#endif
#ifdef INT64_MAX
#if !defined(PRId64) || !defined(PRIi64) || !defined(PRIo64) || !defined(PRIu64) || !defined(PRIx64) || !defined(PRIX64)
#error "a PRI macro for the 64 types is not defined although the type is provided"
#endif
#endif
#ifdef INT_LEAST8_MAX
#if !defined(PRIdLEAST8) || !defined(PRIiLEAST8) || !defined(PRIoLEAST8) || !defined(PRIuLEAST8) || !defined(PRIxLEAST8) || !defined(PRIXLEAST8)
#error "a PRI macro for the LEAST8 types is not defined although the type is provided"
#endif
#endif
#ifdef INT_LEAST16_MAX
#if !defined(PRIdLEAST16) || !defined(PRIiLEAST16) || !defined(PRIoLEAST16) || !defined(PRIuLEAST16) || !defined(PRIxLEAST16) || !defined(PRIXLEAST16)
#error "a PRI macro for the LEAST16 types is not defined although the type is provided"
#endif
#endif
#ifdef INT_LEAST32_MAX
#if !defined(PRIdLEAST32) || !defined(PRIiLEAST32) || !defined(PRIoLEAST32) || !defined(PRIuLEAST32) || !defined(PRIxLEAST32) || !defined(PRIXLEAST32)
#error "a PRI macro for the LEAST32 types is not defined although the type is provided"
#endif
#endif
#ifdef INT_LEAST64_MAX
#if !defined(PRIdLEAST64) || !defined(PRIiLEAST64) || !defined(PRIoLEAST64) || !defined(PRIuLEAST64) || !defined(PRIxLEAST64) || !defined(PRIXLEAST64)
#error "a PRI macro for the LEAST64 types is not defined although the type is provided"
#endif
#endif
#ifdef INT_FAST8_MAX
#if !defined(PRIdFAST8) || !defined(PRIiFAST8) || !defined(PRIoFAST8) || !defined(PRIuFAST8) || !defined(PRIxFAST8) || !defined(PRIXFAST8)
#error "a PRI macro for the FAST8 types is not defined although the type is provided"
#endif
#endif
#ifdef INT_FAST16_MAX
#if !defined(PRIdFAST16) || !defined(PRIiFAST16) || !defined(PRIoFAST16) || !defined(PRIuFAST16) || !defined(PRIxFAST16) || !defined(PRIXFAST16)
#error "a PRI macro for the FAST16 types is not defined although the type is provided"
#endif
#endif
#ifdef INT_FAST32_MAX
#if !defined(PRIdFAST32) || !defined(PRIiFAST32) || !defined(PRIoFAST32) || !defined(PRIuFAST32) || !defined(PRIxFAST32) || !defined(PRIXFAST32)
#error "a PRI macro for the FAST32 types is not defined although the type is provided"
#endif
#endif
#ifdef INT_FAST64_MAX
#if !defined(PRIdFAST64) || !defined(PRIiFAST64) || !defined(PRIoFAST64) || !defined(PRIuFAST64) || !defined(PRIxFAST64) || !defined(PRIXFAST64)
#error "a PRI macro for the FAST64 types is not defined although the type is provided"
#endif
#endif
#ifdef INTMAX_MAX
#if !defined(PRIdMAX) || !defined(PRIiMAX) || !defined(PRIoMAX) || !defined(PRIuMAX) || !defined(PRIxMAX) || !defined(PRIXMAX)
#error "a PRI macro for the MAX types is not defined although the type is provided"
#endif
#endif
#ifdef INTPTR_MAX
#if !defined(PRIdPTR) || !defined(PRIiPTR) || !defined(PRIoPTR) || !defined(PRIuPTR) || !defined(PRIxPTR) || !defined(PRIXPTR)
#error "a PRI macro for the PTR types is not defined although the type is provided"
#endif
#endif

int main(void)
{
	intmax_t m = 0;
	uint32_t u = 0;

	pc_begin("XBD.inttypes.h.001");
	pc_test();
	pc_check(m == 0 && u == 0, "unreachable");
	pc_pass();
}
