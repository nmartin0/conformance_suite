/*
 * Symbols that are always defined and not -1.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.unistd.h.008
 */
#include <unistd.h>
#include "pc.h"
#if !defined(_POSIX_CHOWN_RESTRICTED) || _POSIX_CHOWN_RESTRICTED == -1
#error "_POSIX_CHOWN_RESTRICTED is not defined with a value other than -1"
#endif
#if !defined(_POSIX_NO_TRUNC) || _POSIX_NO_TRUNC == -1
#error "_POSIX_NO_TRUNC is not defined with a value other than -1"
#endif
#if !defined(_POSIX_VDISABLE) || _POSIX_VDISABLE == -1
#error "_POSIX_VDISABLE is not defined with a value other than -1"
#endif
#if !defined(_XOPEN_ENH_I18N) || _XOPEN_ENH_I18N == -1
#error "_XOPEN_ENH_I18N is not defined with a value other than -1"
#endif
#if !defined(_XOPEN_SHM) || _XOPEN_SHM == -1
#error "_XOPEN_SHM is not defined with a value other than -1"
#endif

int main(void)
{
	pc_begin("XBD.unistd.h.008");
	pc_test();
	pc_pass();
}
