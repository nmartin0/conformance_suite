/*
 * The integer constant macros can be used in #if.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.1-use-of-functions.006
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#if !(EOF < 0)
#error "EOF is not negative in #if"
#endif
#if !(BUFSIZ > 0)
#error "BUFSIZ is not positive in #if"
#endif
#if !(RAND_MAX >= 32767)
#error "RAND_MAX is below 32767 in #if"
#endif
#if !(EXIT_FAILURE != EXIT_SUCCESS)
#error "EXIT_FAILURE equals EXIT_SUCCESS in #if"
#endif
#if !(INT_MAX > 0 && INT_MIN < 0)
#error "INT_MAX or INT_MIN has the wrong sign in #if"
#endif
#if !(UINT_MAX > 0)
#error "UINT_MAX is not positive in #if"
#endif
#if !(LONG_MAX > 0 && LONG_MIN < 0)
#error "LONG_MAX or LONG_MIN has the wrong sign in #if"
#endif
#if !(CHAR_BIT > 0)
#error "CHAR_BIT is not positive in #if"
#endif
#if !(SIGINT > 0 && SIGKILL > 0 && SIGTERM > 0)
#error "a signal number is not positive in #if"
#endif
#if !(EINVAL > 0 && ENOENT > 0)
#error "an error number is not positive in #if"
#endif
#if !(SEEK_SET != SEEK_CUR && SEEK_CUR != SEEK_END && SEEK_SET != SEEK_END)
#error "the SEEK_ values are not distinct in #if"
#endif
#if !(FOPEN_MAX > 0)
#error "FOPEN_MAX is not positive in #if"
#endif

int main(void)
{
	pc_begin("XSH.2.1-use-of-functions.006");
	pc_test();
	/* The checks are the #if directives above; they were decided at build time. */
	pc_check(EOF < 0 && SIGINT > 0 && EINVAL > 0, "run-time values disagree with the #if results");
	pc_pass();
}
