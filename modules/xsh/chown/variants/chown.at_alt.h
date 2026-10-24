/*
 * chown.at_alt.h - conforming variant: AT_FDCWD is -2 and
 * AT_SYMLINK_NOFOLLOW is 0x4000 (CI only; with variants/chown.c).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * XBD <fcntl.h> requires these symbolic constants but leaves their values
 * to the implementation; AT_FDCWD has only to differ from every file
 * descriptor, which a negative value does. Included before the test, so
 * the test sees these values; the preload library makes fchownat() agree.
 */
#include <fcntl.h>
#undef AT_FDCWD
#undef AT_SYMLINK_NOFOLLOW
#define AT_FDCWD (-2)
#define AT_SYMLINK_NOFOLLOW 0x4000
