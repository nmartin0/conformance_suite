/*
 * On XSI systems _XOPEN_ENH_I18N and _XOPEN_SHM are defined and not -1.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.unistd.h.032
 */
#include <unistd.h>
#include "pc.h"
#if !defined(_XOPEN_ENH_I18N) || _XOPEN_ENH_I18N == -1
#error "_XOPEN_ENH_I18N is not defined with a value other than -1"
#endif
#if !defined(_XOPEN_SHM) || _XOPEN_SHM == -1
#error "_XOPEN_SHM is not defined with a value other than -1"
#endif

int main(void)
{
	pc_begin("XBD.unistd.h.032");
	pc_test();
	pc_pass();
}
