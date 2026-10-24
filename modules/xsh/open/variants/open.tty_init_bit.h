/*
 * open.tty_init_bit.h - conforming variant: O_TTY_INIT is a non-zero flag
 * of its own (CI only; with variants/open.c).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * XBD <fcntl.h> lets O_TTY_INIT be zero or a bitwise-distinct file
 * creation flag. This value is unused by the real flags here; the preload
 * library removes it before the real call.
 */
#include <fcntl.h>
#undef O_TTY_INIT
#define O_TTY_INIT 0x04000000
