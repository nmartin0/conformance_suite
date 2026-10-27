/*
 * sysconf values of the optional-family options.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.unistd.h.010
 */
#include <unistd.h>
#include "pc.h"

int main(void)
{
	long r;

	pc_begin("XBD.unistd.h.010");
	pc_test();
	r = sysconf(_SC_ADVISORY_INFO);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_ADVISORY_INFO) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_CPUTIME);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_CPUTIME) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_DEVICE_CONTROL);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_DEVICE_CONTROL) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_FSYNC);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_FSYNC) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_IPV6);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_IPV6) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_MEMLOCK);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_MEMLOCK) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_MEMLOCK_RANGE);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_MEMLOCK_RANGE) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_MESSAGE_PASSING);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_MESSAGE_PASSING) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_PRIORITIZED_IO);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_PRIORITIZED_IO) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_PRIORITY_SCHEDULING);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_PRIORITY_SCHEDULING) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_RAW_SOCKETS);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_RAW_SOCKETS) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_SHARED_MEMORY_OBJECTS);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_SHARED_MEMORY_OBJECTS) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_SPAWN);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_SPAWN) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_SPORADIC_SERVER);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_SPORADIC_SERVER) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_SYNCHRONIZED_IO);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_SYNCHRONIZED_IO) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_THREAD_ATTR_STACKADDR);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_THREAD_ATTR_STACKADDR) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_THREAD_ATTR_STACKSIZE);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_THREAD_ATTR_STACKSIZE) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_THREAD_CPUTIME);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_THREAD_CPUTIME) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_THREAD_PRIO_INHERIT);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_THREAD_PRIO_INHERIT) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_THREAD_PRIO_PROTECT);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_THREAD_PRIO_PROTECT) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_THREAD_PRIORITY_SCHEDULING);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_THREAD_PRIORITY_SCHEDULING) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_THREAD_PROCESS_SHARED);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_THREAD_PROCESS_SHARED) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_THREAD_ROBUST_PRIO_INHERIT);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_THREAD_ROBUST_PRIO_INHERIT) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_THREAD_ROBUST_PRIO_PROTECT);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_THREAD_ROBUST_PRIO_PROTECT) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_THREAD_SPORADIC_SERVER);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_THREAD_SPORADIC_SERVER) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_TYPED_MEMORY_OBJECTS);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_TYPED_MEMORY_OBJECTS) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_2_C_DEV);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_2_C_DEV) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_2_FORT_RUN);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_2_FORT_RUN) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_2_LOCALEDEF);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_2_LOCALEDEF) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_2_SW_DEV);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_2_SW_DEV) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_2_UPE);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_2_UPE) is %ld, neither -1 nor 202405L", r);
	r = sysconf(_SC_XOPEN_UUCP);
	pc_check(r == -1 || r == 202405L, "sysconf(_SC_XOPEN_UUCP) is %ld, neither -1 nor 202405L", r);
	pc_pass();
}
