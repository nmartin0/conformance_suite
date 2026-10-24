/*
 * fstat.c - conforming variants of the stat family (fstat(), stat(),
 * lstat(), fstatat()), for the variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. A variant models the whole
 * system, so every function that fills a struct stat is varied the same
 * way. Each variant says why the standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "pcvariant.h"

static int depth;	/* a libc that calls one of these from another: vary once */

/*
 * "type_bits": the file type is encoded in bits 16 to 19 of st_mode
 * instead of 12 to 15 (the header half is fstat.type_bits.h, which moves
 * S_IFMT, the S_IF* values and the S_IS* macros to match). XBD
 * <sys/stat.h> fixes the numeric values of the permission bits but not of
 * the file type constants (S_IFMT and S_IF* are XSI, with no values given),
 * so a conforming application tests the type only with the S_IS* macros or
 * against S_IF* names.
 */
static mode_t move_type(mode_t m)
{
	mode_t t = (m & 0170000) >> 12;

	return (m & ~(mode_t)0170000) | (t << 16);
}

static void cook(const char *v, struct stat *buf)
{
	/*
	 * "time_1s": timestamps have one-second resolution. XBD 4.12 makes the
	 * resolution implementation-defined, no coarser than one second, and
	 * a timestamp set to V takes the greatest supported value not above V,
	 * so every tv_nsec is 0 on such a file system.
	 */
	if (IS("time_1s")) {
		buf->st_atim.tv_nsec = 0;
		buf->st_mtim.tv_nsec = 0;
		buf->st_ctim.tv_nsec = 0;
	}
	if (IS("type_bits"))
		buf->st_mode = move_type(buf->st_mode);
}

/*
 * "shm_partial": for a shared memory object, fstat() updates only st_uid,
 * st_gid, st_size and st_mode, leaving the rest of *buf as the caller left
 * it, and the execute/search bits in st_mode are not meaningful (here they
 * are inverted). XSH fstat() [SHM] requires only those four members and
 * only the read and write permission bits to be valid.
 */
#define NSHM 1024
static unsigned char is_shm[NSHM];

int shm_open(const char *name, int oflag, mode_t mode)
{
	const char *v = variant_of("fstat.");
	int fd;
	REAL(shm_open);

	fd = real_shm_open(name, oflag, mode);
	if (fd >= 0 && fd < NSHM)
		is_shm[fd] = IS("shm_partial");
	return fd;
}

int close(int fd)
{
	REAL(close);

	if (fd >= 0 && fd < NSHM)
		is_shm[fd] = 0;
	return real_close(fd);
}

int fstat(int fd, struct stat *buf)
{
	const char *v = variant_of("fstat.");
	struct stat st;
	int r;
	REAL(fstat);

	if (v == NULL || depth)
		return real_fstat(fd, buf);
	depth++;
	r = real_fstat(fd, &st);
	depth--;
	if (r != 0)
		return r;
	if (IS("shm_partial") && fd >= 0 && fd < NSHM && is_shm[fd]) {
		buf->st_uid = st.st_uid;
		buf->st_gid = st.st_gid;
		buf->st_size = st.st_size;
		buf->st_mode = st.st_mode ^ 0111;
		return 0;
	}
	*buf = st;
	cook(v, buf);
	return 0;
}

int stat(const char *restrict path, struct stat *restrict buf)
{
	const char *v = variant_of("fstat.");
	int r;
	REAL(stat);

	if (v == NULL || depth)
		return real_stat(path, buf);
	depth++;
	r = real_stat(path, buf);
	depth--;
	if (r == 0)
		cook(v, buf);
	return r;
}

int lstat(const char *restrict path, struct stat *restrict buf)
{
	const char *v = variant_of("fstat.");
	int r;
	REAL(lstat);

	if (v == NULL || depth)
		return real_lstat(path, buf);
	depth++;
	r = real_lstat(path, buf);
	depth--;
	if (r == 0)
		cook(v, buf);
	return r;
}

int fstatat(int fd, const char *restrict path, struct stat *restrict buf, int flag)
{
	const char *v = variant_of("fstat.");
	int r;
	REAL(fstatat);

	if (v == NULL || depth)
		return real_fstatat(fd, path, buf, flag);
	depth++;
	r = real_fstatat(fd, path, buf, flag);
	depth--;
	if (r == 0)
		cook(v, buf);
	return r;
}
