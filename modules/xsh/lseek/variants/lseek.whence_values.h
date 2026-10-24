/*
 * lseek.whence_values.h - conforming variant: the whence constants have
 * unusual values, 10 to 14 (CI only; with variants/lseek.c).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * XBD <stdio.h> asks only that SEEK_CUR, SEEK_END and SEEK_SET be distinct
 * integer constant expressions, and XBD <unistd.h> that SEEK_HOLE and
 * SEEK_DATA be distinct from each other and from those three (systems in
 * use already differ: SEEK_DATA is 3 on some and 4 on others). Included
 * before the test, so the test sees these values; the preload library
 * makes lseek() agree, and this system's usual values 0 to 4 are then
 * ordinary invalid whence values.
 */
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#undef SEEK_SET
#undef SEEK_CUR
#undef SEEK_END
#undef SEEK_HOLE
#undef SEEK_DATA
#define SEEK_SET 10
#define SEEK_CUR 11
#define SEEK_END 12
#define SEEK_DATA 13
#define SEEK_HOLE 14
