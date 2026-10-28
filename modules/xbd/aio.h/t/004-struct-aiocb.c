/*
 * <aio.h> struct aiocb
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.aio.h.004
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

typedef char pc_c0[sizeof(struct aiocb)];	/* a complete type */
static struct aiocb *ps;

int main(void)
{
	pc_begin("XBD.aio.h.004");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->aio_fildes, int *))
		pc_fail("struct aiocb member aio_fildes is not of type int");
	if (!IS_T(&ps->aio_offset, off_t *))
		pc_fail("struct aiocb member aio_offset is not of type off_t");
	if (!IS_T(&ps->aio_buf, volatile void * *))
		pc_fail("struct aiocb member aio_buf is not of type volatile void *");
	if (!IS_T(&ps->aio_nbytes, size_t *))
		pc_fail("struct aiocb member aio_nbytes is not of type size_t");
	if (!IS_T(&ps->aio_reqprio, int *))
		pc_fail("struct aiocb member aio_reqprio is not of type int");
	if (!IS_T(&ps->aio_sigevent, struct sigevent *))
		pc_fail("struct aiocb member aio_sigevent is not of type struct sigevent");
	if (!IS_T(&ps->aio_lio_opcode, int *))
		pc_fail("struct aiocb member aio_lio_opcode is not of type int");
	pc_pass();
}
