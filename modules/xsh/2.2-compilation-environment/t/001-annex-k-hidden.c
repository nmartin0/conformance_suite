/*
 * Annex K names are not visible without __STDC_WANT_LIB_EXT1__.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.2-compilation-environment.001
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>
#include "pc.h"

#if defined(__STDC_WANT_LIB_EXT1__) && (__STDC_WANT_LIB_EXT1__ + 0) != 0
#error "the feature test macro is set by the build; the test needs it unset"
#endif

#ifdef RSIZE_MAX
#error "RSIZE_MAX is defined without __STDC_WANT_LIB_EXT1__"
#endif
#ifdef L_tmpnam_s
#error "L_tmpnam_s is defined without __STDC_WANT_LIB_EXT1__"
#endif
#ifdef TMP_MAX_S
#error "TMP_MAX_S is defined without __STDC_WANT_LIB_EXT1__"
#endif

/* The program's own objects; a header that declared one of the names makes this fail to build. */
static int fopen_s = 1;
static int fprintf_s = 1;
static int freopen_s = 1;
static int fscanf_s = 1;
static int gets_s = 1;
static int printf_s = 1;
static int scanf_s = 1;
static int snprintf_s = 1;
static int sprintf_s = 1;
static int sscanf_s = 1;
static int tmpfile_s = 1;
static int tmpnam_s = 1;
static int vfprintf_s = 1;
static int vfscanf_s = 1;
static int vprintf_s = 1;
static int vscanf_s = 1;
static int vsnprintf_s = 1;
static int vsprintf_s = 1;
static int vsscanf_s = 1;
static int abort_handler_s = 1;
static int bsearch_s = 1;
static int getenv_s = 1;
static int ignore_handler_s = 1;
static int mbstowcs_s = 1;
static int qsort_s = 1;
static int set_constraint_handler_s = 1;
static int wctomb_s = 1;
static int asctime_s = 1;
static int ctime_s = 1;
static int gmtime_s = 1;
static int localtime_s = 1;
static int fwprintf_s = 1;
static int fwscanf_s = 1;
static int snwprintf_s = 1;
static int swprintf_s = 1;
static int swscanf_s = 1;
static int vfwprintf_s = 1;
static int vfwscanf_s = 1;
static int vswprintf_s = 1;
static int vswscanf_s = 1;
static int vwprintf_s = 1;
static int vwscanf_s = 1;
static int wprintf_s = 1;
static int wscanf_s = 1;

int main(void)
{
	int sum = 0;

	pc_begin("XSH.2.2-compilation-environment.001");
	pc_test();
	sum += fopen_s;
	sum += fprintf_s;
	sum += freopen_s;
	sum += fscanf_s;
	sum += gets_s;
	sum += printf_s;
	sum += scanf_s;
	sum += snprintf_s;
	sum += sprintf_s;
	sum += sscanf_s;
	sum += tmpfile_s;
	sum += tmpnam_s;
	sum += vfprintf_s;
	sum += vfscanf_s;
	sum += vprintf_s;
	sum += vscanf_s;
	sum += vsnprintf_s;
	sum += vsprintf_s;
	sum += vsscanf_s;
	sum += abort_handler_s;
	sum += bsearch_s;
	sum += getenv_s;
	sum += ignore_handler_s;
	sum += mbstowcs_s;
	sum += qsort_s;
	sum += set_constraint_handler_s;
	sum += wctomb_s;
	sum += asctime_s;
	sum += ctime_s;
	sum += gmtime_s;
	sum += localtime_s;
	sum += fwprintf_s;
	sum += fwscanf_s;
	sum += snwprintf_s;
	sum += swprintf_s;
	sum += swscanf_s;
	sum += vfwprintf_s;
	sum += vfwscanf_s;
	sum += vswprintf_s;
	sum += vswscanf_s;
	sum += vwprintf_s;
	sum += vwscanf_s;
	sum += wprintf_s;
	sum += wscanf_s;
	pc_check(sum == 44, "the program's own objects were changed");
	pc_pass();
}
