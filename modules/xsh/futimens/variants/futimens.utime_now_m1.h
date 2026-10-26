/*
 * futimens.utime_now_m1.h - conforming variant: UTIME_NOW is -1 and
 * UTIME_OMIT is -2 (CI only; with variants/futimens.c).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * XBD <sys/stat.h> requires only that UTIME_NOW and UTIME_OMIT be symbolic
 * constants distinct from each other and from every valid tv_nsec value;
 * a negative value such as -1 is not a valid tv_nsec, so it is allowed
 * (any such pair will do). Included before the test, so the test sees
 * these values; the preload library makes the functions agree.
 */
#include <sys/stat.h>
#undef UTIME_NOW
#undef UTIME_OMIT
#define UTIME_NOW (-1L)
#define UTIME_OMIT (-2L)
