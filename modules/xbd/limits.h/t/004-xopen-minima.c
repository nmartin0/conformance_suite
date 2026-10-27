/*
 * The XSI minimum values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.limits.h.004
 */
#include <limits.h>
#include "pc.h"
#if !defined(_XOPEN_IOV_MAX) || (_XOPEN_IOV_MAX) != 16
#error "_XOPEN_IOV_MAX is missing or not the required value"
#endif
#if !defined(_XOPEN_NAME_MAX) || (_XOPEN_NAME_MAX) != 255
#error "_XOPEN_NAME_MAX is missing or not the required value"
#endif
#if !defined(_XOPEN_PATH_MAX) || (_XOPEN_PATH_MAX) != 1024
#error "_XOPEN_PATH_MAX is missing or not the required value"
#endif

int main(void)
{
	pc_begin("XBD.limits.h.004");
	pc_test();
	pc_pass();
}
