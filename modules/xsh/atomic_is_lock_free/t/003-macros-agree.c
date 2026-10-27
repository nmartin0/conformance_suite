/*
 * A lock-free macro of 2 means atomic_is_lock_free() is true for the
 * signed and the unsigned type; a macro of 0 means it is false.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_is_lock_free.003
 */
#define PCAT_ID "XSH.atomic_is_lock_free.003"
#include "pcat.h"
#ifndef PCAT_NONE

static int constrained;

static void agree(const char *name, int macro, int lf)
{
	if (macro == 2) {
		constrained++;
		pc_check(lf, "%s: the macro is 2 (always lock-free) but atomic_is_lock_free() is false", name);
	} else if (macro == 0) {
		constrained++;
		pc_check(!lf, "%s: the macro is 0 (never lock-free) but atomic_is_lock_free() is true", name);
	}
}

static atomic_bool o_bool;
static atomic_char o_char;
static atomic_schar o_schar;
static atomic_uchar o_uchar;
static atomic_short o_short;
static atomic_ushort o_ushort;
static atomic_int o_int;
static atomic_uint o_uint;
static atomic_long o_long;
static atomic_ulong o_ulong;
static atomic_llong o_llong;
static atomic_ullong o_ullong;
static atomic_char16_t o_c16;
static atomic_char32_t o_c32;
static atomic_wchar_t o_wc;
static _Atomic(void *) o_pv;
static _Atomic(char *) o_pc;
static _Atomic(int *) o_pi;
static _Atomic(long *) o_pl;

#define LF(o) (!!atomic_is_lock_free(&(o)))

int main(void)
{
	pc_begin(PCAT_ID);
	pc_test();
#if defined(ATOMIC_BOOL_LOCK_FREE)
	agree("bool", ATOMIC_BOOL_LOCK_FREE, LF(o_bool));
#endif
#if defined(ATOMIC_CHAR_LOCK_FREE)
	agree("char", ATOMIC_CHAR_LOCK_FREE, LF(o_char));
	agree("signed char", ATOMIC_CHAR_LOCK_FREE, LF(o_schar));
	agree("unsigned char", ATOMIC_CHAR_LOCK_FREE, LF(o_uchar));
#endif
#if defined(ATOMIC_SHORT_LOCK_FREE)
	agree("short", ATOMIC_SHORT_LOCK_FREE, LF(o_short));
	agree("unsigned short", ATOMIC_SHORT_LOCK_FREE, LF(o_ushort));
#endif
#if defined(ATOMIC_INT_LOCK_FREE)
	agree("int", ATOMIC_INT_LOCK_FREE, LF(o_int));
	agree("unsigned int", ATOMIC_INT_LOCK_FREE, LF(o_uint));
#endif
#if defined(ATOMIC_LONG_LOCK_FREE)
	agree("long", ATOMIC_LONG_LOCK_FREE, LF(o_long));
	agree("unsigned long", ATOMIC_LONG_LOCK_FREE, LF(o_ulong));
#endif
#if defined(ATOMIC_LLONG_LOCK_FREE)
	agree("long long", ATOMIC_LLONG_LOCK_FREE, LF(o_llong));
	agree("unsigned long long", ATOMIC_LLONG_LOCK_FREE, LF(o_ullong));
#endif
#if defined(ATOMIC_CHAR16_T_LOCK_FREE)
	agree("char16_t", ATOMIC_CHAR16_T_LOCK_FREE, LF(o_c16));
#endif
#if defined(ATOMIC_CHAR32_T_LOCK_FREE)
	agree("char32_t", ATOMIC_CHAR32_T_LOCK_FREE, LF(o_c32));
#endif
#if defined(ATOMIC_WCHAR_T_LOCK_FREE)
	agree("wchar_t", ATOMIC_WCHAR_T_LOCK_FREE, LF(o_wc));
#endif
#if defined(ATOMIC_POINTER_LOCK_FREE)
	agree("void *", ATOMIC_POINTER_LOCK_FREE, LF(o_pv));
	agree("char *", ATOMIC_POINTER_LOCK_FREE, LF(o_pc));
	agree("int *", ATOMIC_POINTER_LOCK_FREE, LF(o_pi));
	agree("long *", ATOMIC_POINTER_LOCK_FREE, LF(o_pl));
#endif
	if (constrained == 0)
		pc_notinuse("every lock-free macro is 1 (sometimes lock-free): nothing to compare");
	pc_pass();
}
#endif
