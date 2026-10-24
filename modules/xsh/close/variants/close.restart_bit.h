/*
 * close.restart_bit.h - conforming variant: POSIX_CLOSE_RESTART is
 * non-zero (CI only; with variants/close.c).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * XBD <unistd.h> lets POSIX_CLOSE_RESTART be 0 or non-zero. Redefined only
 * where the system declares posix_close() at all; the preload library
 * turns this bit back into the system's own value.
 */
#include <unistd.h>
#ifdef POSIX_CLOSE_RESTART
#undef POSIX_CLOSE_RESTART
#define POSIX_CLOSE_RESTART 0x40000000
#endif
