/*
 * The stdio.h constants are integer constant expressions.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdio.h.001
 */
#include <stdio.h>
#include "pc.h"

_Static_assert((BUFSIZ) == (BUFSIZ), "BUFSIZ is not an integer constant expression");
_Static_assert((L_tmpnam) == (L_tmpnam), "L_tmpnam is not an integer constant expression");
_Static_assert((FILENAME_MAX) == (FILENAME_MAX), "FILENAME_MAX is not an integer constant expression");
_Static_assert((FOPEN_MAX) == (FOPEN_MAX), "FOPEN_MAX is not an integer constant expression");
_Static_assert((TMP_MAX) == (TMP_MAX), "TMP_MAX is not an integer constant expression");
_Static_assert((_IOFBF) == (_IOFBF), "_IOFBF is not an integer constant expression");
_Static_assert((_IOLBF) == (_IOLBF), "_IOLBF is not an integer constant expression");
_Static_assert((_IONBF) == (_IONBF), "_IONBF is not an integer constant expression");
_Static_assert((SEEK_CUR) == (SEEK_CUR), "SEEK_CUR is not an integer constant expression");
_Static_assert((SEEK_END) == (SEEK_END), "SEEK_END is not an integer constant expression");
_Static_assert((SEEK_SET) == (SEEK_SET), "SEEK_SET is not an integer constant expression");
_Static_assert((EOF) == (EOF), "EOF is not an integer constant expression");
_Static_assert(BUFSIZ > 0, "BUFSIZ is not positive");
static char buffer[BUFSIZ];

int main(void)
{
	pc_begin("XBD.stdio.h.001");
	pc_test();
	pc_check(sizeof buffer == BUFSIZ, "BUFSIZ does not size an array");
	pc_pass();
}
