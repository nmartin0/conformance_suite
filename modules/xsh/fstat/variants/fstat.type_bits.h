/*
 * fstat.type_bits.h - conforming variant: the file type lives in bits 16
 * to 19 of st_mode (CI only; with variants/fstat.c).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * XBD <sys/stat.h> gives numeric values for the permission bits only; the
 * file type constants (S_IFMT and S_IF*, XSI) have no values given, and
 * the S_IS* macros are how an application tests the type. Included before
 * the test, so the test sees these values; the preload library makes the
 * stat family agree. The permission bits keep their required values.
 */
#include <sys/stat.h>
#include <fcntl.h>
#undef S_IFMT
#undef S_IFIFO
#undef S_IFCHR
#undef S_IFDIR
#undef S_IFBLK
#undef S_IFREG
#undef S_IFLNK
#undef S_IFSOCK
#undef S_ISFIFO
#undef S_ISCHR
#undef S_ISDIR
#undef S_ISBLK
#undef S_ISREG
#undef S_ISLNK
#undef S_ISSOCK
#define S_IFMT   0xf0000
#define S_IFIFO  0x10000
#define S_IFCHR  0x20000
#define S_IFDIR  0x40000
#define S_IFBLK  0x60000
#define S_IFREG  0x80000
#define S_IFLNK  0xa0000
#define S_IFSOCK 0xc0000
#define S_ISFIFO(m) (((m) & S_IFMT) == S_IFIFO)
#define S_ISCHR(m)  (((m) & S_IFMT) == S_IFCHR)
#define S_ISDIR(m)  (((m) & S_IFMT) == S_IFDIR)
#define S_ISBLK(m)  (((m) & S_IFMT) == S_IFBLK)
#define S_ISREG(m)  (((m) & S_IFMT) == S_IFREG)
#define S_ISLNK(m)  (((m) & S_IFMT) == S_IFLNK)
#define S_ISSOCK(m) (((m) & S_IFMT) == S_IFSOCK)
