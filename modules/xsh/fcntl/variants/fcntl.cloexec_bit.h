/*
 * fcntl.cloexec_bit.h - conforming variant: FD_CLOEXEC is 0x40 (CI only;
 * with variants/fcntl.c).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * XBD <fcntl.h> requires only that the file descriptor flags be
 * bitwise-distinct symbolic constants usable in #if directives; it gives
 * FD_CLOEXEC no value. Included before the test, so the test sees this
 * value; the preload library makes F_GETFD and F_SETFD agree.
 */
#include <fcntl.h>
#undef FD_CLOEXEC
#define FD_CLOEXEC 0x40
