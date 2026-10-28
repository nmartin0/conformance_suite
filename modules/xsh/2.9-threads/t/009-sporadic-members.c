/*
 * With the sporadic server option, struct sched_param has the four
 * sporadic server members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.009
 */
#include "pc.h"
#include <sched.h>
#include <string.h>
#include <time.h>

int main(void)
{
	struct sched_param p;

	pc_begin("XSH.2.9-threads.009");
	memset(&p, 0, sizeof p);
	pc_test();
	p.sched_ss_low_priority = 3;
	p.sched_ss_repl_period.tv_sec = 5;
	p.sched_ss_repl_period.tv_nsec = 7;
	p.sched_ss_init_budget.tv_sec = 1;
	p.sched_ss_init_budget.tv_nsec = 9;
	p.sched_ss_max_repl = 4;
	pc_check(_Generic(p.sched_ss_low_priority, int: 1, default: 0), "sched_ss_low_priority is not an int");
	pc_check(_Generic(p.sched_ss_max_repl, int: 1, default: 0), "sched_ss_max_repl is not an int");
	pc_check(_Generic(p.sched_ss_repl_period, struct timespec: 1, default: 0), "sched_ss_repl_period is not a struct timespec");
	pc_check(_Generic(p.sched_ss_init_budget, struct timespec: 1, default: 0), "sched_ss_init_budget is not a struct timespec");
	pc_check(p.sched_ss_low_priority == 3 && p.sched_ss_max_repl == 4, "an int member did not keep its value");
	pc_check(p.sched_ss_repl_period.tv_sec == 5 && p.sched_ss_repl_period.tv_nsec == 7 &&
	    p.sched_ss_init_budget.tv_sec == 1 && p.sched_ss_init_budget.tv_nsec == 9, "a timespec member did not keep its value");
	pc_pass();
}
