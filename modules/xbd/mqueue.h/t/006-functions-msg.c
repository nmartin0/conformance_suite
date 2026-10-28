/*
 * <mqueue.h> functions msg
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.mqueue.h.006
 *
 * Only <mqueue.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <mqueue.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.mqueue.h.006");
	pc_test();
	if (!IS_T(&(mq_close), int (*)(mqd_t)))
		pc_fail("mq_close() is not declared as int mq_close(mqd_t)");
	if (IS_T(&(mq_close), int (*)(mqd_t, int)))
		pc_fail("mq_close() is declared without a prototype");
	if (!IS_T(&(mq_getattr), int (*)(mqd_t, struct mq_attr *)))
		pc_fail("mq_getattr() is not declared as int mq_getattr(mqd_t, struct mq_attr *)");
	if (IS_T(&(mq_getattr), int (*)(mqd_t, struct mq_attr *, int)))
		pc_fail("mq_getattr() is declared without a prototype");
	if (!IS_T(&(mq_notify), int (*)(mqd_t, const struct sigevent *)))
		pc_fail("mq_notify() is not declared as int mq_notify(mqd_t, const struct sigevent *)");
	if (IS_T(&(mq_notify), int (*)(mqd_t, const struct sigevent *, int)))
		pc_fail("mq_notify() is declared without a prototype");
	if (!IS_T(&(mq_open), mqd_t (*)(const char *, int, ...)))
		pc_fail("mq_open() is not declared as mqd_t mq_open(const char *, int, ...)");
	if (!IS_T(&(mq_receive), ssize_t (*)(mqd_t, char *, size_t, unsigned *)))
		pc_fail("mq_receive() is not declared as ssize_t mq_receive(mqd_t, char *, size_t, unsigned *)");
	if (IS_T(&(mq_receive), ssize_t (*)(mqd_t, char *, size_t, unsigned *, int)))
		pc_fail("mq_receive() is declared without a prototype");
	if (!IS_T(&(mq_send), int (*)(mqd_t, const char *, size_t, unsigned)))
		pc_fail("mq_send() is not declared as int mq_send(mqd_t, const char *, size_t, unsigned)");
	if (IS_T(&(mq_send), int (*)(mqd_t, const char *, size_t, unsigned, int)))
		pc_fail("mq_send() is declared without a prototype");
	if (!IS_T(&(mq_setattr), int (*)(mqd_t, const struct mq_attr *restrict, struct mq_attr *restrict)))
		pc_fail("mq_setattr() is not declared as int mq_setattr(mqd_t, const struct mq_attr *restrict, struct mq_attr *restrict)");
	if (IS_T(&(mq_setattr), int (*)(mqd_t, const struct mq_attr *restrict, struct mq_attr *restrict, int)))
		pc_fail("mq_setattr() is declared without a prototype");
	if (!IS_T(&(mq_timedreceive), ssize_t (*)(mqd_t, char *restrict, size_t, unsigned *restrict, const struct timespec *restrict)))
		pc_fail("mq_timedreceive() is not declared as ssize_t mq_timedreceive(mqd_t, char *restrict, size_t, unsigned *restrict, const struct timespec *restrict)");
	if (IS_T(&(mq_timedreceive), ssize_t (*)(mqd_t, char *restrict, size_t, unsigned *restrict, const struct timespec *restrict, int)))
		pc_fail("mq_timedreceive() is declared without a prototype");
	if (!IS_T(&(mq_timedsend), int (*)(mqd_t, const char *, size_t, unsigned, const struct timespec *)))
		pc_fail("mq_timedsend() is not declared as int mq_timedsend(mqd_t, const char *, size_t, unsigned, const struct timespec *)");
	if (IS_T(&(mq_timedsend), int (*)(mqd_t, const char *, size_t, unsigned, const struct timespec *, int)))
		pc_fail("mq_timedsend() is declared without a prototype");
	if (!IS_T(&(mq_unlink), int (*)(const char *)))
		pc_fail("mq_unlink() is not declared as int mq_unlink(const char *)");
	if (IS_T(&(mq_unlink), int (*)(const char *, int)))
		pc_fail("mq_unlink() is declared without a prototype");
	pc_pass();
}
