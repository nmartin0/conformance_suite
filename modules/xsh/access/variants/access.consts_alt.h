/*
 * access.consts_alt.h - conforming variant: F_OK 8, R_OK 1, W_OK 2,
 * X_OK 4, AT_FDCWD -2, AT_EACCESS 0x4000 (CI only; with variants/access.c).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * XBD <unistd.h> requires only that the four access() constants and the
 * ORs of the permission constants be distinct (F_OK need not be 0); XBD
 * <fcntl.h> leaves the values of AT_FDCWD and AT_EACCESS open. Included
 * before the test, so the test sees these values; the preload library
 * makes access() and faccessat() agree.
 */
#include <fcntl.h>
#include <unistd.h>
#undef F_OK
#undef R_OK
#undef W_OK
#undef X_OK
#undef AT_FDCWD
#undef AT_EACCESS
#define F_OK 8
#define R_OK 1
#define W_OK 2
#define X_OK 4
#define AT_FDCWD (-2)
#define AT_EACCESS 0x4000
