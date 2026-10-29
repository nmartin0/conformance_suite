/*
 * control.c - the ABI control: run the checks against routines that break the ABI.
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * usage: control SCENARIO good|bad
 *
 * Each scenario applies one check of abi/check.h (the same function the
 * tests use) to a routine from abi/arch/i386_control.h. With "bad" the
 * routine breaks the rule and the program must report FAIL; with "good" its
 * conforming twin must give PASS. abi/control.sh runs every scenario both
 * ways. A result line goes to $PC_RESULT, as for a test.
 */
#include "../check.h"

#ifndef ABI_ARCH_I386
int main(void)
{
	pc_begin("ABI.control.none");
	pc_notinuse("the control is for the i386 ABI");
}
#else
#include "../arch/i386_control.h"
#include "../sigtest.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>

void ctl_good_nop(void), ctl_bad_ebx(void), ctl_bad_esi(void), ctl_bad_edi(void), ctl_bad_ebp(void);
void ctl_bad_df(void), ctl_bad_cw(void), ctl_bad_mxcsr(void), ctl_bad_x87_leak(void);
void ctl_good_fp1(void), ctl_bad_fp_none(void), ctl_bad_fp_two(void);
void ctl_good_struct(void), ctl_bad_struct_regs(void), ctl_bad_struct_nopop(void);
void ctl_good_ll(void), ctl_bad_ll(void);
void ctl_good_fpcaller(void (*f)(void)), ctl_bad_fpcaller(void (*f)(void));
void ctl_good_caller(void (*f)(void)), ctl_bad_caller_misaligned(void (*f)(void)), ctl_bad_caller_df(void (*f)(void));

static int bad;                 /* 1: use the routine that breaks the rule */

static void run(struct abi_call *c, void (*good)(void), void (*badf)(void))
{
	abi_call_init(c, (void *)(size_t)(bad ? badf : good));
}

/* ---- routines called through abi_call_raw ---- */

static void sc_callee_saved(void (*badf)(void), const char *what)
{
	struct abi_call c;

	run(&c, ctl_good_nop, badf);
	abi_call(&c);
	abi_expect_callee_saved(&c, what);
}

static void sc_df(void)
{
	struct abi_call c;

	run(&c, ctl_good_nop, ctl_bad_df);
	abi_call(&c);
	abi_expect_df_clear(&c, "the routine");
}

static void sc_cw(void)
{
	struct abi_call c;

	run(&c, ctl_good_nop, ctl_bad_cw);
	abi_call(&c);
	abi_expect_x87_cw_kept(&c, "the routine");
}

static void sc_mxcsr(void)
{
	struct abi_call c;

	abi_require_sse();
	run(&c, ctl_good_nop, ctl_bad_mxcsr);
	abi_call(&c);
	abi_expect_mxcsr_kept(&c, "the routine");
}

static void sc_x87_leak(void)
{
	struct abi_call c;

	run(&c, ctl_good_nop, ctl_bad_x87_leak);
	abi_call(&c);
	abi_expect_x87_empty_after(&c, "the routine");
}

static void sc_fp_return(void (*badf)(void))
{
	struct abi_call c;

	run(&c, ctl_good_fp1, badf);
	abi_call(&c);
	abi_expect_st0(&c, 1.0L, "the routine");
}

static void sc_struct(void (*badf)(void))
{
	struct abi_call c;
	unsigned buf[4] = { 0, 0, 0, 0 };

	run(&c, ctl_good_struct, badf);
	abi_call_arg(&c, (unsigned)(size_t)buf);
	abi_call(&c);
	abi_expect_hidden_pointer(&c, buf, "the routine");
}

static void sc_ll(void)
{
	struct abi_call c;

	run(&c, ctl_good_ll, ctl_bad_ll);
	abi_call(&c);
	abi_expect_edx_eax(&c, 0x100000005ULL, "the routine");
}

/* A caller that ignores a floating-point return value must still pop it. */
static void sc_not_popped(void)
{
	if (bad)
		ctl_bad_fpcaller((void (*)(void))ctl_good_fp1);
	else
		ctl_good_fpcaller((void (*)(void))ctl_good_fp1);
	pc_check(abi_x87_used() == 0, "%u x87 register(s) in use after the caller ignored a return value", abi_x87_used());
}

/* ---- entry state of a callee ---- */

ABI_TARGET void ctl_body(void)
{
}
void ctl_tramp(void);
ABI_ENTRY_TRAMPOLINE(ctl_tramp, ctl_body);

static void sc_call_misaligned_primitive(void)
{
	struct abi_call c;

	abi_call_init(&c, (void *)(size_t)ctl_tramp);
	c.misalign = bad ? 4 : 0;
	abi_call(&c);
	abi_expect_entry_mod16(&ctl_tramp_rec, "the callee");
}

static void sc_call_misaligned_caller(void)
{
	if (bad)
		ctl_bad_caller_misaligned((void (*)(void))ctl_tramp);
	else
		ctl_good_caller((void (*)(void))ctl_tramp);
	abi_expect_entry_mod16(&ctl_tramp_rec, "the callee");
}

static void sc_call_df_caller(void)
{
	if (bad)
		ctl_bad_caller_df((void (*)(void))ctl_tramp);
	else
		ctl_good_caller((void (*)(void))ctl_tramp);
	abi_expect_entry_df_clear(&ctl_tramp_rec, "the callee");
}

/* ---- resuming interrupted code: a handler that damages the saved context ---- */

static void damaging_handler(int sig, siginfo_t *si, void *ucv)
{
	ucontext_t *uc = ucv;

	(void)si;
	sig_got = sig;
	if (!bad)
		return;
#ifdef REG_EBX
	uc->uc_mcontext.gregs[REG_EBX] ^= 1;
#else
	(void)uc;
#endif
}

static void sc_resume_regs(void)
{
	struct sigaction sa;
	struct abi_spin s;

#ifndef REG_EBX
	pc_unsupported("the saved register context cannot be reached by name here");
#endif
	memset(&sa, 0, sizeof sa);
	sa.sa_sigaction = damaging_handler;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) != 0)
		pc_unresolved("sigaction failed");
	sig_async(&s, 0, 0);
	abi_expect_resumed_regs(&s, "after the handler");
}

static void damaging_cw_handler(int sig, siginfo_t *si, void *ucv)
{
	ucontext_t *uc = ucv;

	(void)si;
	sig_got = sig;
	if (bad && uc->uc_mcontext.fpregs != NULL)
		uc->uc_mcontext.fpregs->cw = 0x027F;
}

static void sc_resume_cw(void)
{
	struct sigaction sa;
	struct abi_spin s;

	memset(&sa, 0, sizeof sa);
	sa.sa_sigaction = damaging_cw_handler;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) != 0)
		pc_unresolved("sigaction failed");
	sig_async(&s, 0, 0);
	abi_expect_resumed_cw(&s, "after the handler");
}

/* ---- the comparators alone, on records made by hand ---- */

static void sc_synth_resume(void)
{
	struct abi_spin s;

	memset(&s, 0, sizeof s);
	s.in_reg[0] = s.out_reg[0] = 1;
	s.cw_before = s.cw_after = 0x37F;
	s.mxcsr_before = s.mxcsr_after = 0x1F80;
	if (bad) {
		/* each of these alone must be caught; the first failing check ends the program, so use argv */
		const char *w = getenv("ABI_CTL_WHICH");

		if (w && strcmp(w, "mxcsr") == 0)
			s.mxcsr_after ^= 0x2000;
		else if (w && strcmp(w, "df") == 0)
			s.eflags_after |= ABI_EFLAGS_DF;
		else if (w && strcmp(w, "cw") == 0)
			s.cw_after ^= 0x0400;
		else
			s.out_reg[0] ^= 0x10;
	}
	abi_expect_resumed_regs(&s, "the record");
	abi_expect_resumed_cw(&s, "the record");
	abi_expect_resumed_mxcsr(&s, "the record");
	abi_expect_resumed_df(&s, 0, "the record");
}

static const struct {
	const char *name;
	void (*fn)(void);
} scen[] = {
	{ "saved-ebx", NULL }, { "saved-esi", NULL }, { "saved-edi", NULL }, { "saved-ebp", NULL },
	{ "df-set", sc_df }, { "x87-cw-changed", sc_cw }, { "mxcsr-changed", sc_mxcsr },
	{ "x87-leak", sc_x87_leak }, { "x87-not-popped", sc_not_popped }, { "fp-none", NULL }, { "fp-two", NULL },
	{ "struct-in-regs", NULL }, { "struct-no-pop", NULL }, { "ll-low-only", sc_ll },
	{ "stack-misaligned-by-primitive", sc_call_misaligned_primitive },
	{ "stack-misaligned-by-caller", sc_call_misaligned_caller },
	{ "df-set-at-call", sc_call_df_caller },
	{ "resume-regs", sc_resume_regs }, { "resume-x87-cw", sc_resume_cw },
	{ "record-resume", sc_synth_resume },
};

int main(int argc, char **argv)
{
	char id[96];
	size_t i;
	const char *name = argc > 1 ? argv[1] : "";

	(void)snprintf(id, sizeof id, "ABI.control.%s", name);
	pc_begin(id);
	if (argc < 3 || (strcmp(argv[2], "good") != 0 && strcmp(argv[2], "bad") != 0))
		pc_unresolved("usage: control SCENARIO good|bad");
	bad = strcmp(argv[2], "bad") == 0;
	pc_test();
	for (i = 0; i < sizeof scen / sizeof scen[0]; i++) {
		if (strcmp(scen[i].name, name) != 0)
			continue;
		if (strcmp(name, "saved-ebx") == 0) sc_callee_saved(ctl_bad_ebx, "the routine");
		else if (strcmp(name, "saved-esi") == 0) sc_callee_saved(ctl_bad_esi, "the routine");
		else if (strcmp(name, "saved-edi") == 0) sc_callee_saved(ctl_bad_edi, "the routine");
		else if (strcmp(name, "saved-ebp") == 0) sc_callee_saved(ctl_bad_ebp, "the routine");
		else if (strcmp(name, "fp-none") == 0) sc_fp_return(ctl_bad_fp_none);
		else if (strcmp(name, "fp-two") == 0) sc_fp_return(ctl_bad_fp_two);
		else if (strcmp(name, "struct-in-regs") == 0) sc_struct(ctl_bad_struct_regs);
		else if (strcmp(name, "struct-no-pop") == 0) sc_struct(ctl_bad_struct_nopop);
		else scen[i].fn();
		pc_pass();
	}
	pc_unresolved("no scenario named %s", name);
}
#endif
