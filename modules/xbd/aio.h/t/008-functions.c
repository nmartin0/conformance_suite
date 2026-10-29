/*
 * <aio.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.aio.h.008
 *
 * Only <aio.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <aio.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.aio.h.008");
	pc_test();
	if (!IS_T(&(aio_cancel), int (*)(int, struct aiocb *)))
		pc_fail("aio_cancel() is not declared as int aio_cancel(int, struct aiocb *)");
	if (IS_T(&(aio_cancel), int (*)(int, struct aiocb *, int)))
		pc_fail("aio_cancel() is declared without a prototype");
	if (!IS_T(&(aio_error), int (*)(const struct aiocb *)))
		pc_fail("aio_error() is not declared as int aio_error(const struct aiocb *)");
	if (IS_T(&(aio_error), int (*)(const struct aiocb *, int)))
		pc_fail("aio_error() is declared without a prototype");
	if (!IS_T(&(aio_read), int (*)(struct aiocb *)))
		pc_fail("aio_read() is not declared as int aio_read(struct aiocb *)");
	if (IS_T(&(aio_read), int (*)(struct aiocb *, int)))
		pc_fail("aio_read() is declared without a prototype");
	if (!IS_T(&(aio_return), ssize_t (*)(struct aiocb *)))
		pc_fail("aio_return() is not declared as ssize_t aio_return(struct aiocb *)");
	if (IS_T(&(aio_return), ssize_t (*)(struct aiocb *, int)))
		pc_fail("aio_return() is declared without a prototype");
	if (!IS_T(&(aio_suspend), int (*)(const struct aiocb *const [], int, const struct timespec *)))
		pc_fail("aio_suspend() is not declared as int aio_suspend(const struct aiocb *const [], int, const struct timespec *)");
	if (IS_T(&(aio_suspend), int (*)(const struct aiocb *const [], int, const struct timespec *, int)))
		pc_fail("aio_suspend() is declared without a prototype");
	if (!IS_T(&(aio_write), int (*)(struct aiocb *)))
		pc_fail("aio_write() is not declared as int aio_write(struct aiocb *)");
	if (IS_T(&(aio_write), int (*)(struct aiocb *, int)))
		pc_fail("aio_write() is declared without a prototype");
	/* restrict on the elements of the list changes nothing a caller can observe (Q-209) */
	if (!IS_T(&(lio_listio), int (*)(int, struct aiocb *restrict const [restrict], int, struct sigevent *restrict)) &&
	    !IS_T(&(lio_listio), int (*)(int, struct aiocb *const [restrict], int, struct sigevent *restrict)))
		pc_fail("lio_listio() is not declared as int lio_listio(int, struct aiocb *restrict const [restrict], int, struct sigevent *restrict)");
	if (IS_T(&(lio_listio), int (*)(int, struct aiocb *restrict const [restrict], int, struct sigevent *restrict, int)))
		pc_fail("lio_listio() is declared without a prototype");
	pc_pass();
}
