/*
 * The ATOMIC_*_LOCK_FREE macros exist, can be used in #if, and are 0, 1
 * or 2.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_is_lock_free.002
 */
#define PCAT_ID "XSH.atomic_is_lock_free.002"
#include "pcat.h"
#ifndef PCAT_NONE

#ifdef ATOMIC_BOOL_LOCK_FREE
#if ATOMIC_BOOL_LOCK_FREE == 0 || ATOMIC_BOOL_LOCK_FREE == 1 || ATOMIC_BOOL_LOCK_FREE == 2
#define OK_BOOL 1
#endif
#endif
#ifndef OK_BOOL
#define OK_BOOL 0
#endif
#ifdef ATOMIC_CHAR_LOCK_FREE
#if ATOMIC_CHAR_LOCK_FREE == 0 || ATOMIC_CHAR_LOCK_FREE == 1 || ATOMIC_CHAR_LOCK_FREE == 2
#define OK_CHAR 1
#endif
#endif
#ifndef OK_CHAR
#define OK_CHAR 0
#endif
#ifdef ATOMIC_CHAR16_T_LOCK_FREE
#if ATOMIC_CHAR16_T_LOCK_FREE == 0 || ATOMIC_CHAR16_T_LOCK_FREE == 1 || ATOMIC_CHAR16_T_LOCK_FREE == 2
#define OK_CHAR16 1
#endif
#endif
#ifndef OK_CHAR16
#define OK_CHAR16 0
#endif
#ifdef ATOMIC_CHAR32_T_LOCK_FREE
#if ATOMIC_CHAR32_T_LOCK_FREE == 0 || ATOMIC_CHAR32_T_LOCK_FREE == 1 || ATOMIC_CHAR32_T_LOCK_FREE == 2
#define OK_CHAR32 1
#endif
#endif
#ifndef OK_CHAR32
#define OK_CHAR32 0
#endif
#ifdef ATOMIC_WCHAR_T_LOCK_FREE
#if ATOMIC_WCHAR_T_LOCK_FREE == 0 || ATOMIC_WCHAR_T_LOCK_FREE == 1 || ATOMIC_WCHAR_T_LOCK_FREE == 2
#define OK_WCHAR 1
#endif
#endif
#ifndef OK_WCHAR
#define OK_WCHAR 0
#endif
#ifdef ATOMIC_SHORT_LOCK_FREE
#if ATOMIC_SHORT_LOCK_FREE == 0 || ATOMIC_SHORT_LOCK_FREE == 1 || ATOMIC_SHORT_LOCK_FREE == 2
#define OK_SHORT 1
#endif
#endif
#ifndef OK_SHORT
#define OK_SHORT 0
#endif
#ifdef ATOMIC_INT_LOCK_FREE
#if ATOMIC_INT_LOCK_FREE == 0 || ATOMIC_INT_LOCK_FREE == 1 || ATOMIC_INT_LOCK_FREE == 2
#define OK_INT 1
#endif
#endif
#ifndef OK_INT
#define OK_INT 0
#endif
#ifdef ATOMIC_LONG_LOCK_FREE
#if ATOMIC_LONG_LOCK_FREE == 0 || ATOMIC_LONG_LOCK_FREE == 1 || ATOMIC_LONG_LOCK_FREE == 2
#define OK_LONG 1
#endif
#endif
#ifndef OK_LONG
#define OK_LONG 0
#endif
#ifdef ATOMIC_LLONG_LOCK_FREE
#if ATOMIC_LLONG_LOCK_FREE == 0 || ATOMIC_LLONG_LOCK_FREE == 1 || ATOMIC_LLONG_LOCK_FREE == 2
#define OK_LLONG 1
#endif
#endif
#ifndef OK_LLONG
#define OK_LLONG 0
#endif
#ifdef ATOMIC_POINTER_LOCK_FREE
#if ATOMIC_POINTER_LOCK_FREE == 0 || ATOMIC_POINTER_LOCK_FREE == 1 || ATOMIC_POINTER_LOCK_FREE == 2
#define OK_POINTER 1
#endif
#endif
#ifndef OK_POINTER
#define OK_POINTER 0
#endif

int main(void)
{
	pc_begin(PCAT_ID);
	pc_test();
	pc_check(OK_BOOL, "ATOMIC_BOOL_LOCK_FREE is missing or not 0, 1 or 2 in #if");
	pc_check(OK_CHAR, "ATOMIC_CHAR_LOCK_FREE is missing or not 0, 1 or 2 in #if");
	pc_check(OK_CHAR16, "ATOMIC_CHAR16_T_LOCK_FREE is missing or not 0, 1 or 2 in #if");
	pc_check(OK_CHAR32, "ATOMIC_CHAR32_T_LOCK_FREE is missing or not 0, 1 or 2 in #if");
	pc_check(OK_WCHAR, "ATOMIC_WCHAR_T_LOCK_FREE is missing or not 0, 1 or 2 in #if");
	pc_check(OK_SHORT, "ATOMIC_SHORT_LOCK_FREE is missing or not 0, 1 or 2 in #if");
	pc_check(OK_INT, "ATOMIC_INT_LOCK_FREE is missing or not 0, 1 or 2 in #if");
	pc_check(OK_LONG, "ATOMIC_LONG_LOCK_FREE is missing or not 0, 1 or 2 in #if");
	pc_check(OK_LLONG, "ATOMIC_LLONG_LOCK_FREE is missing or not 0, 1 or 2 in #if");
	pc_check(OK_POINTER, "ATOMIC_POINTER_LOCK_FREE is missing or not 0, 1 or 2 in #if");
	pc_pass();
}
#endif
