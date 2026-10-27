/*
 * The message-queue minimum values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.limits.h.002
 */
#include <limits.h>
#include "pc.h"
#if !defined(_POSIX_MQ_OPEN_MAX) || (_POSIX_MQ_OPEN_MAX) != 8
#error "_POSIX_MQ_OPEN_MAX is missing or not the required value"
#endif
#if !defined(_POSIX_MQ_PRIO_MAX) || (_POSIX_MQ_PRIO_MAX) != 32
#error "_POSIX_MQ_PRIO_MAX is missing or not the required value"
#endif

int main(void)
{
	pc_begin("XBD.limits.h.002");
	pc_test();
	pc_pass();
}
