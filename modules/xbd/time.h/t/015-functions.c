/*
 * <time.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.time.h.015
 *
 * Only <time.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <time.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.time.h.015");
	pc_test();
	if (!IS_T(&(asctime), char *(*)(const struct tm *)))
		pc_fail("asctime() is not declared as char *asctime(const struct tm *)");
	if (IS_T(&(asctime), char *(*)(const struct tm *, int)))
		pc_fail("asctime() is declared without a prototype");
	if (!IS_T(&(clock), clock_t (*)(void)))
		pc_fail("clock() is not declared as clock_t clock(void)");
	if (IS_T(&(clock), clock_t (*)(int)))
		pc_fail("clock() is declared without a prototype");
	if (!IS_T(&(clock_getres), int (*)(clockid_t, struct timespec *)))
		pc_fail("clock_getres() is not declared as int clock_getres(clockid_t, struct timespec *)");
	if (IS_T(&(clock_getres), int (*)(clockid_t, struct timespec *, int)))
		pc_fail("clock_getres() is declared without a prototype");
	if (!IS_T(&(clock_gettime), int (*)(clockid_t, struct timespec *)))
		pc_fail("clock_gettime() is not declared as int clock_gettime(clockid_t, struct timespec *)");
	if (IS_T(&(clock_gettime), int (*)(clockid_t, struct timespec *, int)))
		pc_fail("clock_gettime() is declared without a prototype");
	if (!IS_T(&(clock_nanosleep), int (*)(clockid_t, int, const struct timespec *, struct timespec *)))
		pc_fail("clock_nanosleep() is not declared as int clock_nanosleep(clockid_t, int, const struct timespec *, struct timespec *)");
	if (IS_T(&(clock_nanosleep), int (*)(clockid_t, int, const struct timespec *, struct timespec *, int)))
		pc_fail("clock_nanosleep() is declared without a prototype");
	if (!IS_T(&(clock_settime), int (*)(clockid_t, const struct timespec *)))
		pc_fail("clock_settime() is not declared as int clock_settime(clockid_t, const struct timespec *)");
	if (IS_T(&(clock_settime), int (*)(clockid_t, const struct timespec *, int)))
		pc_fail("clock_settime() is declared without a prototype");
	if (!IS_T(&(ctime), char *(*)(const time_t *)))
		pc_fail("ctime() is not declared as char *ctime(const time_t *)");
	if (IS_T(&(ctime), char *(*)(const time_t *, int)))
		pc_fail("ctime() is declared without a prototype");
	if (!IS_T(&(difftime), double (*)(time_t, time_t)))
		pc_fail("difftime() is not declared as double difftime(time_t, time_t)");
	if (IS_T(&(difftime), double (*)(time_t, time_t, int)))
		pc_fail("difftime() is declared without a prototype");
	if (!IS_T(&(gmtime), struct tm *(*)(const time_t *)))
		pc_fail("gmtime() is not declared as struct tm *gmtime(const time_t *)");
	if (IS_T(&(gmtime), struct tm *(*)(const time_t *, int)))
		pc_fail("gmtime() is declared without a prototype");
	if (!IS_T(&(gmtime_r), struct tm *(*)(const time_t *restrict, struct tm *restrict)))
		pc_fail("gmtime_r() is not declared as struct tm *gmtime_r(const time_t *restrict, struct tm *restrict)");
	if (IS_T(&(gmtime_r), struct tm *(*)(const time_t *restrict, struct tm *restrict, int)))
		pc_fail("gmtime_r() is declared without a prototype");
	if (!IS_T(&(localtime), struct tm *(*)(const time_t *)))
		pc_fail("localtime() is not declared as struct tm *localtime(const time_t *)");
	if (IS_T(&(localtime), struct tm *(*)(const time_t *, int)))
		pc_fail("localtime() is declared without a prototype");
	if (!IS_T(&(localtime_r), struct tm *(*)(const time_t *restrict, struct tm *restrict)))
		pc_fail("localtime_r() is not declared as struct tm *localtime_r(const time_t *restrict, struct tm *restrict)");
	if (IS_T(&(localtime_r), struct tm *(*)(const time_t *restrict, struct tm *restrict, int)))
		pc_fail("localtime_r() is declared without a prototype");
	if (!IS_T(&(mktime), time_t (*)(struct tm *)))
		pc_fail("mktime() is not declared as time_t mktime(struct tm *)");
	if (IS_T(&(mktime), time_t (*)(struct tm *, int)))
		pc_fail("mktime() is declared without a prototype");
	if (!IS_T(&(nanosleep), int (*)(const struct timespec *, struct timespec *)))
		pc_fail("nanosleep() is not declared as int nanosleep(const struct timespec *, struct timespec *)");
	if (IS_T(&(nanosleep), int (*)(const struct timespec *, struct timespec *, int)))
		pc_fail("nanosleep() is declared without a prototype");
	if (!IS_T(&(strftime), size_t (*)(char *restrict, size_t, const char *restrict, const struct tm *restrict)))
		pc_fail("strftime() is not declared as size_t strftime(char *restrict, size_t, const char *restrict, const struct tm *restrict)");
	if (IS_T(&(strftime), size_t (*)(char *restrict, size_t, const char *restrict, const struct tm *restrict, int)))
		pc_fail("strftime() is declared without a prototype");
	if (!IS_T(&(strftime_l), size_t (*)(char *restrict, size_t, const char *restrict, const struct tm *restrict, locale_t)))
		pc_fail("strftime_l() is not declared as size_t strftime_l(char *restrict, size_t, const char *restrict, const struct tm *restrict, locale_t)");
	if (IS_T(&(strftime_l), size_t (*)(char *restrict, size_t, const char *restrict, const struct tm *restrict, locale_t, int)))
		pc_fail("strftime_l() is declared without a prototype");
	if (!IS_T(&(time), time_t (*)(time_t *)))
		pc_fail("time() is not declared as time_t time(time_t *)");
	if (IS_T(&(time), time_t (*)(time_t *, int)))
		pc_fail("time() is declared without a prototype");
	if (!IS_T(&(timer_create), int (*)(clockid_t, struct sigevent *restrict, timer_t *restrict)))
		pc_fail("timer_create() is not declared as int timer_create(clockid_t, struct sigevent *restrict, timer_t *restrict)");
	if (IS_T(&(timer_create), int (*)(clockid_t, struct sigevent *restrict, timer_t *restrict, int)))
		pc_fail("timer_create() is declared without a prototype");
	if (!IS_T(&(timer_delete), int (*)(timer_t)))
		pc_fail("timer_delete() is not declared as int timer_delete(timer_t)");
	if (IS_T(&(timer_delete), int (*)(timer_t, int)))
		pc_fail("timer_delete() is declared without a prototype");
	if (!IS_T(&(timer_getoverrun), int (*)(timer_t)))
		pc_fail("timer_getoverrun() is not declared as int timer_getoverrun(timer_t)");
	if (IS_T(&(timer_getoverrun), int (*)(timer_t, int)))
		pc_fail("timer_getoverrun() is declared without a prototype");
	if (!IS_T(&(timer_gettime), int (*)(timer_t, struct itimerspec *)))
		pc_fail("timer_gettime() is not declared as int timer_gettime(timer_t, struct itimerspec *)");
	if (IS_T(&(timer_gettime), int (*)(timer_t, struct itimerspec *, int)))
		pc_fail("timer_gettime() is declared without a prototype");
	if (!IS_T(&(timer_settime), int (*)(timer_t, int, const struct itimerspec *restrict, struct itimerspec *restrict)))
		pc_fail("timer_settime() is not declared as int timer_settime(timer_t, int, const struct itimerspec *restrict, struct itimerspec *restrict)");
	if (IS_T(&(timer_settime), int (*)(timer_t, int, const struct itimerspec *restrict, struct itimerspec *restrict, int)))
		pc_fail("timer_settime() is declared without a prototype");
	if (!IS_T(&(tzset), void (*)(void)))
		pc_fail("tzset() is not declared as void tzset(void)");
	if (IS_T(&(tzset), void (*)(int)))
		pc_fail("tzset() is declared without a prototype");
	pc_pass();
}
