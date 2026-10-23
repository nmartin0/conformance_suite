/*
 * fseek.c - planted faults in fseek() and fseeko(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "<function>.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process or a parent).
 */
#define _GNU_SOURCE 1
#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static char phase[16];

/* pc.c sets PC_PHASE with setenv(); remember it in this process. */
int setenv(const char *name, const char *value, int overwrite)
{
	static int (*real)(const char *, const char *, int);

	if (real == NULL)
		real = (int (*)(const char *, const char *, int))dlsym(RTLD_NEXT, "setenv");
	if (strcmp(name, "PC_PHASE") == 0 && strlen(value) < sizeof phase)
		strcpy(phase, value);
	return real(name, value, overwrite);
}

static const char *mutant_of(const char *prefix)
{
	const char *m = getenv("PC_MUTANT");
	size_t n = strlen(prefix);

	if (strcmp(phase, "test") != 0 || m == NULL || strncmp(m, prefix, n) != 0)
		return NULL;
	return m + n;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

#include <stdio.h>
#include <stdio_ext.h>
#include <sys/types.h>

static FILE *eof_f;

/* One fault applied to a seek; does the real one through real_fseeko(). */
static int seek(FILE *f, off_t off, int whence, int narrow)
{
	REAL(fseeko);
	const char *m = mutant_of("fseek.");
	struct stat before;
	off_t fdpos = -1;
	int was_eof = feof(f), r, have = 0;

	if (m == NULL)
		return real_fseeko(f, off, whence);
	if (narrow && IS("fseeko_narrow"))
		off = (off_t)(int)off;
	if (IS("whence_set_only"))
		whence = SEEK_SET;
	if (IS("cur_ignores_buffer") && whence == SEEK_CUR && fileno(f) >= 0) {
		off += lseek(fileno(f), 0, SEEK_CUR);	/* counts from the descriptor, past the read-ahead */
		whence = SEEK_SET;
	}
	if (IS("end_off_by_one") && whence == SEEK_END)
		off -= 1;
	if (IS("drops_pending"))
		__fpurge(f);	/* buffered output is thrown away, not written */
	if (IS("bad_whence_ok") && whence != SEEK_SET && whence != SEEK_CUR && whence != SEEK_END)
		return 0;
	if (IS("negative_ok") && whence == SEEK_CUR && ftello(f) + off < 0)
		return 0;
	if (IS("beyond_end_refused") && fileno(f) >= 0 && fstat(fileno(f), &before) == 0 && whence == SEEK_SET &&
	    off > before.st_size) {
		errno = EINVAL;
		return -1;
	}
	if (IS("mem_beyond_ok") && fileno(f) < 0)
		return 0;
	if (IS("fd_offset_stale") && fileno(f) >= 0)
		fdpos = lseek(fileno(f), 0, SEEK_CUR);
	if ((IS("old_times") || IS("gap_filled")) && fileno(f) >= 0)
		have = fstat(fileno(f), &before) == 0;
	r = real_fseeko(f, off, whence);
	if (r == 0 && IS("keeps_eof") && was_eof)
		eof_f = f;
	if (r == 0 && fdpos != -1)
		(void)lseek(fileno(f), fdpos, SEEK_SET);
	if (r == 0 && IS("gap_filled") && have) {
		off_t at = ftello(f), i;

		for (i = before.st_size; i < at; i++)
			(void)pwrite(fileno(f), "G", 1, i);
	}
	if (r == 0 && IS("old_times") && have) {
		struct timespec t[2];

		t[0] = before.st_atim;
		t[1] = before.st_mtim;
		(void)futimens(fileno(f), t);
	}
	if (r != 0 && IS("espipe_ok") && errno == ESPIPE)
		return 0;
	if (r != 0 && IS("error_cleared"))
		clearerr(f);
	if (r != 0 && IS("wrong_errno"))
		errno = errno == EIO ? ENXIO : EIO;
	return r;
}

int fseek(FILE *f, long off, int whence)
{
	REAL(fseek);
	const char *m = mutant_of("fseek.");

	if (m == NULL)
		return real_fseek(f, off, whence);
	if (IS("wrong_errno") || IS("whence_set_only") || IS("cur_ignores_buffer") || IS("end_off_by_one") ||
	    IS("drops_pending") || IS("bad_whence_ok") || IS("negative_ok") || IS("beyond_end_refused") ||
	    IS("mem_beyond_ok") || IS("fd_offset_stale") || IS("old_times") || IS("gap_filled") || IS("keeps_eof") ||
	    IS("espipe_ok") || IS("error_cleared")) {
		int r;
		if (IS("wrong_errno") || IS("error_cleared") || IS("espipe_ok")) {
			r = real_fseek(f, off, whence);	/* keep the real long-offset checks (EOVERFLOW) */
			if (r != 0 && IS("espipe_ok") && errno == ESPIPE)
				return 0;
			if (r != 0 && IS("error_cleared"))
				clearerr(f);
			if (r != 0 && IS("wrong_errno"))
				errno = errno == EIO ? ENXIO : EIO;
			return r;
		}
		return seek(f, off, whence, 0);
	}
	return real_fseek(f, off, whence);
}

int fseeko(FILE *f, off_t off, int whence)
{
	return seek(f, off, whence, 1);
}

int feof(FILE *f)
{
	REAL(feof);
	const char *m = mutant_of("fseek.");

	if (IS("keeps_eof") && f == eof_f)
		return 1;	/* the indicator still reads as set after the seek */
	return real_feof(f);
}

int fgetc(FILE *f)
{
	REAL(fgetc);

	if (f == eof_f)
		eof_f = NULL;
	return real_fgetc(f);
}
