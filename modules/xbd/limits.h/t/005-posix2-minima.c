/*
 * The fixed _POSIX2_* minimum values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.limits.h.005
 */
#include <limits.h>
#include "pc.h"
#if !defined(_POSIX2_BC_BASE_MAX) || (_POSIX2_BC_BASE_MAX) != 99
#error "_POSIX2_BC_BASE_MAX is missing or not the required value"
#endif
#if !defined(_POSIX2_BC_DIM_MAX) || (_POSIX2_BC_DIM_MAX) != 2048
#error "_POSIX2_BC_DIM_MAX is missing or not the required value"
#endif
#if !defined(_POSIX2_BC_SCALE_MAX) || (_POSIX2_BC_SCALE_MAX) != 99
#error "_POSIX2_BC_SCALE_MAX is missing or not the required value"
#endif
#if !defined(_POSIX2_BC_STRING_MAX) || (_POSIX2_BC_STRING_MAX) != 1000
#error "_POSIX2_BC_STRING_MAX is missing or not the required value"
#endif
#if !defined(_POSIX2_CHARCLASS_NAME_MAX) || (_POSIX2_CHARCLASS_NAME_MAX) != 14
#error "_POSIX2_CHARCLASS_NAME_MAX is missing or not the required value"
#endif
#if !defined(_POSIX2_COLL_WEIGHTS_MAX) || (_POSIX2_COLL_WEIGHTS_MAX) != 2
#error "_POSIX2_COLL_WEIGHTS_MAX is missing or not the required value"
#endif
#if !defined(_POSIX2_EXPR_NEST_MAX) || (_POSIX2_EXPR_NEST_MAX) != 32
#error "_POSIX2_EXPR_NEST_MAX is missing or not the required value"
#endif
#if !defined(_POSIX2_LINE_MAX) || (_POSIX2_LINE_MAX) != 2048
#error "_POSIX2_LINE_MAX is missing or not the required value"
#endif
#if !defined(_POSIX2_RE_DUP_MAX) || (_POSIX2_RE_DUP_MAX) != 255
#error "_POSIX2_RE_DUP_MAX is missing or not the required value"
#endif

int main(void)
{
	pc_begin("XBD.limits.h.005");
	pc_test();
	pc_pass();
}
