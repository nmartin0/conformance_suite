/*
 * FE_DFL_ENV has type pointer to const fenv_t and can be installed with
 *   fesetenv().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fenv.h.005
 */
#include <fenv.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

#pragma STDC FENV_ACCESS ON

int main(void)
{
	fenv_t saved;
	fexcept_t flags;

	pc_begin("XBD.fenv.h.005");
	pc_test();
	pc_check(IS_T(FE_DFL_ENV, const fenv_t *), "FE_DFL_ENV does not have type const fenv_t *");
	pc_check(sizeof saved > 0 && sizeof flags > 0, "fenv_t or fexcept_t is not a complete object type");
	pc_check(fegetenv(&saved) == 0, "fegetenv() failed");
	pc_check(fesetenv(FE_DFL_ENV) == 0, "fesetenv(FE_DFL_ENV) failed");
	pc_check(fesetenv(&saved) == 0, "fesetenv() of the saved environment failed");
	pc_pass();
}
