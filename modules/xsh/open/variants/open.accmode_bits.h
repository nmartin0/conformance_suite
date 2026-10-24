/*
 * open.accmode_bits.h - conforming variant: the file access modes are
 * separate non-zero bits (CI only; with variants/open.c).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * XBD <fcntl.h> requires the access modes only to be unique (O_EXEC and
 * O_SEARCH may share a value) and O_ACCMODE to mask them; O_RDONLY need
 * not be 0, and the open() RATIONALE hopes for read and write bits with
 * O_RDONLY|O_WRONLY equal to O_RDWR. These values are unused by the real
 * flags here; the preload library translates them.
 */
#include <fcntl.h>
#undef O_RDONLY
#undef O_WRONLY
#undef O_RDWR
#undef O_ACCMODE
#define O_RDONLY 0x10000000
#define O_WRONLY 0x20000000
#define O_RDWR 0x30000000
#ifdef O_SEARCH
#undef O_SEARCH
#undef O_EXEC
#define O_SEARCH 0x40000000
#define O_EXEC 0x40000000
#define O_ACCMODE 0x70000000
#else
#define O_ACCMODE 0x30000000
#endif
