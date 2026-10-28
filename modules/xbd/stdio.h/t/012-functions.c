/*
 * <stdio.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdio.h.012
 *
 * Only <stdio.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <stdio.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.stdio.h.012");
	pc_test();
	if (!IS_T(&(clearerr), void (*)(FILE *)))
		pc_fail("clearerr() is not declared as void clearerr(FILE *)");
	if (IS_T(&(clearerr), void (*)(FILE *, int)))
		pc_fail("clearerr() is declared without a prototype");
	if (!IS_T(&(ctermid), char *(*)(char *)))
		pc_fail("ctermid() is not declared as char *ctermid(char *)");
	if (IS_T(&(ctermid), char *(*)(char *, int)))
		pc_fail("ctermid() is declared without a prototype");
	if (!IS_T(&(dprintf), int (*)(int, const char *restrict, ...)))
		pc_fail("dprintf() is not declared as int dprintf(int, const char *restrict, ...)");
	if (!IS_T(&(fclose), int (*)(FILE *)))
		pc_fail("fclose() is not declared as int fclose(FILE *)");
	if (IS_T(&(fclose), int (*)(FILE *, int)))
		pc_fail("fclose() is declared without a prototype");
	if (!IS_T(&(fdopen), FILE *(*)(int, const char *)))
		pc_fail("fdopen() is not declared as FILE *fdopen(int, const char *)");
	if (IS_T(&(fdopen), FILE *(*)(int, const char *, int)))
		pc_fail("fdopen() is declared without a prototype");
	if (!IS_T(&(feof), int (*)(FILE *)))
		pc_fail("feof() is not declared as int feof(FILE *)");
	if (IS_T(&(feof), int (*)(FILE *, int)))
		pc_fail("feof() is declared without a prototype");
	if (!IS_T(&(ferror), int (*)(FILE *)))
		pc_fail("ferror() is not declared as int ferror(FILE *)");
	if (IS_T(&(ferror), int (*)(FILE *, int)))
		pc_fail("ferror() is declared without a prototype");
	if (!IS_T(&(fflush), int (*)(FILE *)))
		pc_fail("fflush() is not declared as int fflush(FILE *)");
	if (IS_T(&(fflush), int (*)(FILE *, int)))
		pc_fail("fflush() is declared without a prototype");
	if (!IS_T(&(fgetc), int (*)(FILE *)))
		pc_fail("fgetc() is not declared as int fgetc(FILE *)");
	if (IS_T(&(fgetc), int (*)(FILE *, int)))
		pc_fail("fgetc() is declared without a prototype");
	if (!IS_T(&(fgetpos), int (*)(FILE *restrict, fpos_t *restrict)))
		pc_fail("fgetpos() is not declared as int fgetpos(FILE *restrict, fpos_t *restrict)");
	if (IS_T(&(fgetpos), int (*)(FILE *restrict, fpos_t *restrict, int)))
		pc_fail("fgetpos() is declared without a prototype");
	if (!IS_T(&(fgets), char *(*)(char *restrict, int, FILE *restrict)))
		pc_fail("fgets() is not declared as char *fgets(char *restrict, int, FILE *restrict)");
	if (IS_T(&(fgets), char *(*)(char *restrict, int, FILE *restrict, int)))
		pc_fail("fgets() is declared without a prototype");
	if (!IS_T(&(fileno), int (*)(FILE *)))
		pc_fail("fileno() is not declared as int fileno(FILE *)");
	if (IS_T(&(fileno), int (*)(FILE *, int)))
		pc_fail("fileno() is declared without a prototype");
	if (!IS_T(&(flockfile), void (*)(FILE *)))
		pc_fail("flockfile() is not declared as void flockfile(FILE *)");
	if (IS_T(&(flockfile), void (*)(FILE *, int)))
		pc_fail("flockfile() is declared without a prototype");
	if (!IS_T(&(fmemopen), FILE *(*)(void *restrict, size_t, const char *restrict)))
		pc_fail("fmemopen() is not declared as FILE *fmemopen(void *restrict, size_t, const char *restrict)");
	if (IS_T(&(fmemopen), FILE *(*)(void *restrict, size_t, const char *restrict, int)))
		pc_fail("fmemopen() is declared without a prototype");
	if (!IS_T(&(fopen), FILE *(*)(const char *restrict, const char *restrict)))
		pc_fail("fopen() is not declared as FILE *fopen(const char *restrict, const char *restrict)");
	if (IS_T(&(fopen), FILE *(*)(const char *restrict, const char *restrict, int)))
		pc_fail("fopen() is declared without a prototype");
	if (!IS_T(&(fprintf), int (*)(FILE *restrict, const char *restrict, ...)))
		pc_fail("fprintf() is not declared as int fprintf(FILE *restrict, const char *restrict, ...)");
	if (!IS_T(&(fputc), int (*)(int, FILE *)))
		pc_fail("fputc() is not declared as int fputc(int, FILE *)");
	if (IS_T(&(fputc), int (*)(int, FILE *, int)))
		pc_fail("fputc() is declared without a prototype");
	if (!IS_T(&(fputs), int (*)(const char *restrict, FILE *restrict)))
		pc_fail("fputs() is not declared as int fputs(const char *restrict, FILE *restrict)");
	if (IS_T(&(fputs), int (*)(const char *restrict, FILE *restrict, int)))
		pc_fail("fputs() is declared without a prototype");
	if (!IS_T(&(fread), size_t (*)(void *restrict, size_t, size_t, FILE *restrict)))
		pc_fail("fread() is not declared as size_t fread(void *restrict, size_t, size_t, FILE *restrict)");
	if (IS_T(&(fread), size_t (*)(void *restrict, size_t, size_t, FILE *restrict, int)))
		pc_fail("fread() is declared without a prototype");
	if (!IS_T(&(freopen), FILE *(*)(const char *restrict, const char *restrict, FILE *restrict)))
		pc_fail("freopen() is not declared as FILE *freopen(const char *restrict, const char *restrict, FILE *restrict)");
	if (IS_T(&(freopen), FILE *(*)(const char *restrict, const char *restrict, FILE *restrict, int)))
		pc_fail("freopen() is declared without a prototype");
	if (!IS_T(&(fscanf), int (*)(FILE *restrict, const char *restrict, ...)))
		pc_fail("fscanf() is not declared as int fscanf(FILE *restrict, const char *restrict, ...)");
	if (!IS_T(&(fseek), int (*)(FILE *, long, int)))
		pc_fail("fseek() is not declared as int fseek(FILE *, long, int)");
	if (IS_T(&(fseek), int (*)(FILE *, long, int, int)))
		pc_fail("fseek() is declared without a prototype");
	if (!IS_T(&(fseeko), int (*)(FILE *, off_t, int)))
		pc_fail("fseeko() is not declared as int fseeko(FILE *, off_t, int)");
	if (IS_T(&(fseeko), int (*)(FILE *, off_t, int, int)))
		pc_fail("fseeko() is declared without a prototype");
	if (!IS_T(&(fsetpos), int (*)(FILE *, const fpos_t *)))
		pc_fail("fsetpos() is not declared as int fsetpos(FILE *, const fpos_t *)");
	if (IS_T(&(fsetpos), int (*)(FILE *, const fpos_t *, int)))
		pc_fail("fsetpos() is declared without a prototype");
	if (!IS_T(&(ftell), long (*)(FILE *)))
		pc_fail("ftell() is not declared as long ftell(FILE *)");
	if (IS_T(&(ftell), long (*)(FILE *, int)))
		pc_fail("ftell() is declared without a prototype");
	if (!IS_T(&(ftello), off_t (*)(FILE *)))
		pc_fail("ftello() is not declared as off_t ftello(FILE *)");
	if (IS_T(&(ftello), off_t (*)(FILE *, int)))
		pc_fail("ftello() is declared without a prototype");
	if (!IS_T(&(ftrylockfile), int (*)(FILE *)))
		pc_fail("ftrylockfile() is not declared as int ftrylockfile(FILE *)");
	if (IS_T(&(ftrylockfile), int (*)(FILE *, int)))
		pc_fail("ftrylockfile() is declared without a prototype");
	if (!IS_T(&(funlockfile), void (*)(FILE *)))
		pc_fail("funlockfile() is not declared as void funlockfile(FILE *)");
	if (IS_T(&(funlockfile), void (*)(FILE *, int)))
		pc_fail("funlockfile() is declared without a prototype");
	if (!IS_T(&(fwrite), size_t (*)(const void *restrict, size_t, size_t, FILE *restrict)))
		pc_fail("fwrite() is not declared as size_t fwrite(const void *restrict, size_t, size_t, FILE *restrict)");
	if (IS_T(&(fwrite), size_t (*)(const void *restrict, size_t, size_t, FILE *restrict, int)))
		pc_fail("fwrite() is declared without a prototype");
	if (!IS_T(&(getc), int (*)(FILE *)))
		pc_fail("getc() is not declared as int getc(FILE *)");
	if (IS_T(&(getc), int (*)(FILE *, int)))
		pc_fail("getc() is declared without a prototype");
	if (!IS_T(&(getchar), int (*)(void)))
		pc_fail("getchar() is not declared as int getchar(void)");
	if (IS_T(&(getchar), int (*)(int)))
		pc_fail("getchar() is declared without a prototype");
	if (!IS_T(&(getc_unlocked), int (*)(FILE *)))
		pc_fail("getc_unlocked() is not declared as int getc_unlocked(FILE *)");
	if (IS_T(&(getc_unlocked), int (*)(FILE *, int)))
		pc_fail("getc_unlocked() is declared without a prototype");
	if (!IS_T(&(getchar_unlocked), int (*)(void)))
		pc_fail("getchar_unlocked() is not declared as int getchar_unlocked(void)");
	if (IS_T(&(getchar_unlocked), int (*)(int)))
		pc_fail("getchar_unlocked() is declared without a prototype");
	if (!IS_T(&(getdelim), ssize_t (*)(char **restrict, size_t *restrict, int, FILE *restrict)))
		pc_fail("getdelim() is not declared as ssize_t getdelim(char **restrict, size_t *restrict, int, FILE *restrict)");
	if (IS_T(&(getdelim), ssize_t (*)(char **restrict, size_t *restrict, int, FILE *restrict, int)))
		pc_fail("getdelim() is declared without a prototype");
	if (!IS_T(&(getline), ssize_t (*)(char **restrict, size_t *restrict, FILE *restrict)))
		pc_fail("getline() is not declared as ssize_t getline(char **restrict, size_t *restrict, FILE *restrict)");
	if (IS_T(&(getline), ssize_t (*)(char **restrict, size_t *restrict, FILE *restrict, int)))
		pc_fail("getline() is declared without a prototype");
	if (!IS_T(&(open_memstream), FILE *(*)(char **, size_t *)))
		pc_fail("open_memstream() is not declared as FILE *open_memstream(char **, size_t *)");
	if (IS_T(&(open_memstream), FILE *(*)(char **, size_t *, int)))
		pc_fail("open_memstream() is declared without a prototype");
	if (!IS_T(&(pclose), int (*)(FILE *)))
		pc_fail("pclose() is not declared as int pclose(FILE *)");
	if (IS_T(&(pclose), int (*)(FILE *, int)))
		pc_fail("pclose() is declared without a prototype");
	if (!IS_T(&(perror), void (*)(const char *)))
		pc_fail("perror() is not declared as void perror(const char *)");
	if (IS_T(&(perror), void (*)(const char *, int)))
		pc_fail("perror() is declared without a prototype");
	if (!IS_T(&(popen), FILE *(*)(const char *, const char *)))
		pc_fail("popen() is not declared as FILE *popen(const char *, const char *)");
	if (IS_T(&(popen), FILE *(*)(const char *, const char *, int)))
		pc_fail("popen() is declared without a prototype");
	if (!IS_T(&(printf), int (*)(const char *restrict, ...)))
		pc_fail("printf() is not declared as int printf(const char *restrict, ...)");
	if (!IS_T(&(putc), int (*)(int, FILE *)))
		pc_fail("putc() is not declared as int putc(int, FILE *)");
	if (IS_T(&(putc), int (*)(int, FILE *, int)))
		pc_fail("putc() is declared without a prototype");
	if (!IS_T(&(putchar), int (*)(int)))
		pc_fail("putchar() is not declared as int putchar(int)");
	if (IS_T(&(putchar), int (*)(int, int)))
		pc_fail("putchar() is declared without a prototype");
	if (!IS_T(&(putc_unlocked), int (*)(int, FILE *)))
		pc_fail("putc_unlocked() is not declared as int putc_unlocked(int, FILE *)");
	if (IS_T(&(putc_unlocked), int (*)(int, FILE *, int)))
		pc_fail("putc_unlocked() is declared without a prototype");
	if (!IS_T(&(putchar_unlocked), int (*)(int)))
		pc_fail("putchar_unlocked() is not declared as int putchar_unlocked(int)");
	if (IS_T(&(putchar_unlocked), int (*)(int, int)))
		pc_fail("putchar_unlocked() is declared without a prototype");
	if (!IS_T(&(puts), int (*)(const char *)))
		pc_fail("puts() is not declared as int puts(const char *)");
	if (IS_T(&(puts), int (*)(const char *, int)))
		pc_fail("puts() is declared without a prototype");
	if (!IS_T(&(remove), int (*)(const char *)))
		pc_fail("remove() is not declared as int remove(const char *)");
	if (IS_T(&(remove), int (*)(const char *, int)))
		pc_fail("remove() is declared without a prototype");
	if (!IS_T(&(rename), int (*)(const char *, const char *)))
		pc_fail("rename() is not declared as int rename(const char *, const char *)");
	if (IS_T(&(rename), int (*)(const char *, const char *, int)))
		pc_fail("rename() is declared without a prototype");
	if (!IS_T(&(renameat), int (*)(int, const char *, int, const char *)))
		pc_fail("renameat() is not declared as int renameat(int, const char *, int, const char *)");
	if (IS_T(&(renameat), int (*)(int, const char *, int, const char *, int)))
		pc_fail("renameat() is declared without a prototype");
	if (!IS_T(&(rewind), void (*)(FILE *)))
		pc_fail("rewind() is not declared as void rewind(FILE *)");
	if (IS_T(&(rewind), void (*)(FILE *, int)))
		pc_fail("rewind() is declared without a prototype");
	if (!IS_T(&(scanf), int (*)(const char *restrict, ...)))
		pc_fail("scanf() is not declared as int scanf(const char *restrict, ...)");
	if (!IS_T(&(setbuf), void (*)(FILE *restrict, char *restrict)))
		pc_fail("setbuf() is not declared as void setbuf(FILE *restrict, char *restrict)");
	if (IS_T(&(setbuf), void (*)(FILE *restrict, char *restrict, int)))
		pc_fail("setbuf() is declared without a prototype");
	if (!IS_T(&(setvbuf), int (*)(FILE *restrict, char *restrict, int, size_t)))
		pc_fail("setvbuf() is not declared as int setvbuf(FILE *restrict, char *restrict, int, size_t)");
	if (IS_T(&(setvbuf), int (*)(FILE *restrict, char *restrict, int, size_t, int)))
		pc_fail("setvbuf() is declared without a prototype");
	if (!IS_T(&(snprintf), int (*)(char *restrict, size_t, const char *restrict, ...)))
		pc_fail("snprintf() is not declared as int snprintf(char *restrict, size_t, const char *restrict, ...)");
	if (!IS_T(&(sprintf), int (*)(char *restrict, const char *restrict, ...)))
		pc_fail("sprintf() is not declared as int sprintf(char *restrict, const char *restrict, ...)");
	if (!IS_T(&(sscanf), int (*)(const char *restrict, const char *restrict, ...)))
		pc_fail("sscanf() is not declared as int sscanf(const char *restrict, const char *restrict, ...)");
	if (!IS_T(&(tmpfile), FILE *(*)(void)))
		pc_fail("tmpfile() is not declared as FILE *tmpfile(void)");
	if (IS_T(&(tmpfile), FILE *(*)(int)))
		pc_fail("tmpfile() is declared without a prototype");
	if (!IS_T(&(tmpnam), char *(*)(char *)))
		pc_fail("tmpnam() is not declared as char *tmpnam(char *)");
	if (IS_T(&(tmpnam), char *(*)(char *, int)))
		pc_fail("tmpnam() is declared without a prototype");
	if (!IS_T(&(ungetc), int (*)(int, FILE *)))
		pc_fail("ungetc() is not declared as int ungetc(int, FILE *)");
	if (IS_T(&(ungetc), int (*)(int, FILE *, int)))
		pc_fail("ungetc() is declared without a prototype");
	if (!IS_T(&(vdprintf), int (*)(int, const char *restrict, va_list)))
		pc_fail("vdprintf() is not declared as int vdprintf(int, const char *restrict, va_list)");
	if (IS_T(&(vdprintf), int (*)(int, const char *restrict, va_list, int)))
		pc_fail("vdprintf() is declared without a prototype");
	if (!IS_T(&(vfprintf), int (*)(FILE *restrict, const char *restrict, va_list)))
		pc_fail("vfprintf() is not declared as int vfprintf(FILE *restrict, const char *restrict, va_list)");
	if (IS_T(&(vfprintf), int (*)(FILE *restrict, const char *restrict, va_list, int)))
		pc_fail("vfprintf() is declared without a prototype");
	if (!IS_T(&(vfscanf), int (*)(FILE *restrict, const char *restrict, va_list)))
		pc_fail("vfscanf() is not declared as int vfscanf(FILE *restrict, const char *restrict, va_list)");
	if (IS_T(&(vfscanf), int (*)(FILE *restrict, const char *restrict, va_list, int)))
		pc_fail("vfscanf() is declared without a prototype");
	if (!IS_T(&(vprintf), int (*)(const char *restrict, va_list)))
		pc_fail("vprintf() is not declared as int vprintf(const char *restrict, va_list)");
	if (IS_T(&(vprintf), int (*)(const char *restrict, va_list, int)))
		pc_fail("vprintf() is declared without a prototype");
	if (!IS_T(&(vscanf), int (*)(const char *restrict, va_list)))
		pc_fail("vscanf() is not declared as int vscanf(const char *restrict, va_list)");
	if (IS_T(&(vscanf), int (*)(const char *restrict, va_list, int)))
		pc_fail("vscanf() is declared without a prototype");
	if (!IS_T(&(vsnprintf), int (*)(char *restrict, size_t, const char *restrict, va_list)))
		pc_fail("vsnprintf() is not declared as int vsnprintf(char *restrict, size_t, const char *restrict, va_list)");
	if (IS_T(&(vsnprintf), int (*)(char *restrict, size_t, const char *restrict, va_list, int)))
		pc_fail("vsnprintf() is declared without a prototype");
	if (!IS_T(&(vsprintf), int (*)(char *restrict, const char *restrict, va_list)))
		pc_fail("vsprintf() is not declared as int vsprintf(char *restrict, const char *restrict, va_list)");
	if (IS_T(&(vsprintf), int (*)(char *restrict, const char *restrict, va_list, int)))
		pc_fail("vsprintf() is declared without a prototype");
	if (!IS_T(&(vsscanf), int (*)(const char *restrict, const char *restrict, va_list)))
		pc_fail("vsscanf() is not declared as int vsscanf(const char *restrict, const char *restrict, va_list)");
	if (IS_T(&(vsscanf), int (*)(const char *restrict, const char *restrict, va_list, int)))
		pc_fail("vsscanf() is declared without a prototype");
	pc_pass();
}
