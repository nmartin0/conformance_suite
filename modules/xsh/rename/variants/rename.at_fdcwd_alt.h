/*
 * rename.at_fdcwd_alt.h - conforming variant: AT_FDCWD has another value
 * (CI only; with variants/rename.c).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * XBD <fcntl.h> requires only that AT_FDCWD be a symbolic constant, a
 * special value used in place of a directory descriptor. Its value is not
 * given; being special, it cannot be a descriptor that could be open. Included
 * before the test, so the test sees this value; the preload library makes
 * the functions agree, and this system's own value is then an ordinary
 * descriptor that is not open.
 */
#include <fcntl.h>
#undef AT_FDCWD
#define AT_FDCWD (-3041965)
