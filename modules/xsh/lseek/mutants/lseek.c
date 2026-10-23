/*
 * lseek.c - planted faults in lseek(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "lseek.<fault>" and apply only
 * while PC_PHASE is "test".
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

typedef off_t (*lseek_fn)(int, off_t, int);

static lseek_fn real_lseek(void)
{
	static lseek_fn fn;
	if (fn == NULL)
		fn = (lseek_fn)dlsym(RTLD_NEXT, "lseek");
	return fn;
}

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");
	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL ||
	    strncmp(m, "lseek.", 6) != 0)
		return NULL;
	return m + 6;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)

static off_t faulty(const char *m, int fd, off_t off, int whence)
{
	lseek_fn f = real_lseek();
	off_t cur = f(fd, 0, SEEK_CUR), r;
	struct stat st;
	int e;

	if (IS("noop") && cur != -1) {
		/* Reports the new offset but does not move. */
		r = f(fd, off, whence);
		if (r != -1)
			(void)f(fd, cur, SEEK_SET);
		return r;
	}
	if (IS("set_off_by_one") && whence == SEEK_SET && off > 0)
		off++;
	if (IS("cur_as_set") && whence == SEEK_CUR)
		whence = SEEK_SET;
	if (IS("end_ignores_offset") && whence == SEEK_END)
		off = 0;
	if (IS("bad_whence_ok") && whence != SEEK_SET && whence != SEEK_CUR &&
	    whence != SEEK_END
#ifdef SEEK_DATA
	    && whence != SEEK_DATA && whence != SEEK_HOLE
#endif
	    )
		whence = SEEK_SET;
	if (IS("negative_ok") && whence == SEEK_SET && off < 0)
		off = 0;
	if (IS("negative_cur_ok") && whence == SEEK_CUR && cur != -1 && cur + off < 0)
		return f(fd, 0, SEEK_SET);
#ifdef SEEK_HOLE
	if (IS("hole_at_offset") && whence == SEEK_HOLE &&
	    fstat(fd, &st) == 0 && off < st.st_size)
		return f(fd, off, SEEK_SET);
	if (IS("hole_beyond_ok") && whence == SEEK_HOLE &&
	    fstat(fd, &st) == 0 && off >= st.st_size)
		return f(fd, st.st_size, SEEK_SET);
	if (IS("data_enxio") && whence == SEEK_DATA) {
		errno = ENXIO;
		return -1;
	}
#endif
	r = f(fd, off, whence);
	e = errno;
	if (r == -1) {
		if (IS("error_moves_offset"))
			(void)f(fd, 0, SEEK_SET);
		if (IS("espipe_as_einval") && e == ESPIPE)
			e = EINVAL;
		if (IS("ebadf_as_einval") && e == EBADF)
			e = EINVAL;
		if (IS("einval_as_eoverflow") && e == EINVAL)
			e = EOVERFLOW;
		if (IS("eoverflow_as_einval") && e == EOVERFLOW)
			e = EINVAL;
		if (IS("enxio_as_einval") && e == ENXIO)
			e = EINVAL;
		errno = e;
		return -1;
	}
	if (IS("extends_with_junk") && fstat(fd, &st) == 0 && r > st.st_size) {
		/* Fills the gap: the file grows and the gap is not zeros. */
		off_t i;
		for (i = st.st_size; i < r; i++)
			(void)pwrite(fd, "x", 1, i);
	}
	return r;
}

off_t lseek(int fd, off_t off, int whence)
{
	const char *m = mutant();

	if (m == NULL)
		return real_lseek()(fd, off, whence);
	return faulty(m, fd, off, whence);
}

#ifdef __GLIBC__
off_t lseek64(int fd, off_t off, int whence)
{
	return lseek(fd, off, whence);
}
#endif
