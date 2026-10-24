/*
 * <unistd.h> defines SEEK_SET, SEEK_CUR and SEEK_END as macros with
 * integer constant values, all different.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.unistd.h.003
 */
#include "pc.h"
#include <unistd.h>

int main(void)
{
	pc_begin("XBD.unistd.h.003");
	pc_test();
#if !defined(SEEK_SET) || !defined(SEEK_CUR) || !defined(SEEK_END)
	pc_fail("<unistd.h> does not define SEEK_SET, SEEK_CUR and SEEK_END as macros");
#else
	{
		_Static_assert((SEEK_SET) == (SEEK_SET), "SEEK_SET is an integer constant expression");
		_Static_assert((SEEK_CUR) == (SEEK_CUR), "SEEK_CUR is an integer constant expression");
		_Static_assert((SEEK_END) == (SEEK_END), "SEEK_END is an integer constant expression");
#define S (SEEK_SET)
#define C (SEEK_CUR)
#define E (SEEK_END)
		pc_check(S != C && S != E && C != E, "SEEK_SET, SEEK_CUR and SEEK_END are not all different");
	}
	pc_pass();
#endif
}
