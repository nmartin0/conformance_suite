/*
 * check.h - what the ABI checks look at, written once.
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * Every test includes this header. It selects the architecture header by
 * compiler guard and holds the checks that turn the facts a primitive
 * recorded (abi/arch/ARCH.h) into pc_check() verdicts. It contains no
 * assembly: the same checks run in the tests against library code and in
 * abi/control against deliberately wrong routines, which they must flag.
 *
 * Only the i386 architecture exists so far. When this build is for another
 * architecture, ABI_ARCH_I386 stays undefined and each i386 test uses
 * ABI_NOT_THIS_ARCH() to report NOTINUSE.
 */
#ifndef ABI_CHECK_H
#define ABI_CHECK_H

#include "pc.h"

#if defined(__i386__)
#include "arch/i386.h"
#ifdef ABI_ENTRY_FAULT
#include "arch/i386_entry_fault.h"   /* control only */
#else
#define ABI_ENTRY_PRE_ASM ""
#endif
#ifndef ABI_ENTRY_POST_ASM
#define ABI_ENTRY_POST_ASM ""
#endif
#endif

/* The body of a test built for another architecture. */
#define ABI_NOT_THIS_ARCH(id)                                                \
	int main(void)                                                       \
	{                                                                    \
		pc_begin(id);                                                \
		pc_notinuse("this test is for the i386 ABI; the compiler "   \
		    "builds for another architecture");                      \
	}

#ifdef ABI_ARCH_I386

/* ---------- preconditions (a failed one is UNRESOLVED, not a verdict) ---------- */

static inline void abi_require_sse(void)
{
	if (!abi_cpu_has_sse())
		pc_unresolved("the processor has no SSE, so MXCSR cannot be read");
}

/* The x87 register stack is empty before a call is made (so counts after it mean something). */
static inline void abi_require_x87_empty(const struct abi_call *c)
{
	unsigned tw = (unsigned)c->env_before[8] | ((unsigned)c->env_before[9] << 8);
	if (tw != 0xFFFFu)
		pc_unresolved("x87 stack not empty before the call (tag word 0x%04x)", tw);
}

/* ---------- checks on one recorded call ---------- */

/* EBX, ESI, EDI and EBP hold what the caller put there. */
static inline void abi_expect_callee_saved(const struct abi_call *c, const char *what)
{
	static const char *const name[4] = { "EBX", "ESI", "EDI", "EBP" };
	int i;

	for (i = 0; i < 4; i++)
		pc_check(c->out_reg[i] == c->in_reg[i],
		    "%s changed %s: 0x%08x became 0x%08x", what, name[i],
		    c->in_reg[i], c->out_reg[i]);
}

/* The direction flag is clear on return (it was clear at the call). */
static inline void abi_expect_df_clear(const struct abi_call *c, const char *what)
{
	pc_check((c->eflags_after & ABI_EFLAGS_DF) == 0,
	    "%s returned with the direction flag set (EFLAGS 0x%08x)", what,
	    c->eflags_after);
}

/* The x87 control word is what it was before the call. */
static inline void abi_expect_x87_cw_kept(const struct abi_call *c, const char *what)
{
	pc_check(c->cw_after == c->cw_before,
	    "%s changed the x87 control word: 0x%04x became 0x%04x", what,
	    c->cw_before, c->cw_after);
}

/* The MXCSR control bits are what they were before the call (status bits may change). */
static inline void abi_expect_mxcsr_kept(const struct abi_call *c, const char *what)
{
	pc_check(((c->mxcsr_before ^ c->mxcsr_after) & ABI_MXCSR_CONTROL) == 0,
	    "%s changed the MXCSR control bits: 0x%04x became 0x%04x", what,
	    c->mxcsr_before & ABI_MXCSR_CONTROL,
	    c->mxcsr_after & ABI_MXCSR_CONTROL);
}

/* The x87 register stack holds nothing on return (a function that returns no x87 value). */
static inline void abi_expect_x87_empty_after(const struct abi_call *c, const char *what)
{
	pc_check(c->npop == 0,
	    "%s returned with %u value(s) on the x87 register stack", what, c->npop);
}

/*
 * A function returning an aggregate: the hidden first argument (ptr) comes
 * back in EAX, and the callee has popped it (ESP is 4 higher than at the
 * call).
 */
static inline void abi_expect_hidden_pointer(const struct abi_call *c, const void *ptr,
    const char *what)
{
	pc_check(c->eax == (unsigned)(size_t)ptr,
	    "%s returned 0x%08x in EAX, not the hidden pointer 0x%08x", what,
	    c->eax, (unsigned)(size_t)ptr);
	pc_check(c->esp_after == c->esp_before + 4,
	    "%s left ESP %d bytes from its value at the call; the hidden "
	    "argument should have been popped (+4)", what,
	    (int)(c->esp_after - c->esp_before));
}

/* A function returning a floating-point value: exactly one value was left in st0, equal to v. */
static inline void abi_expect_st0(const struct abi_call *c, long double v, const char *what)
{
	pc_check(c->npop == 1, "%s left %u value(s) on the x87 register stack, expected exactly 1 (in st0)",
	    what, c->npop);
	pc_check(abi_call_st0(c) == v, "%s returned %.20Lg in st0, expected %.20Lg", what,
	    abi_call_st0(c), v);
}

/* A function returning a 64-bit integer: the high half is in EDX, the low half in EAX. */
static inline void abi_expect_edx_eax(const struct abi_call *c, unsigned long long v, const char *what)
{
	pc_check(c->eax == (unsigned)v, "%s returned 0x%08x in EAX, expected 0x%08x", what, c->eax,
	    (unsigned)v);
	pc_check(c->edx == (unsigned)(v >> 32), "%s returned 0x%08x in EDX, expected 0x%08x", what, c->edx,
	    (unsigned)(v >> 32));
}

/* ---------- checks on code that was interrupted and resumed ---------- */

/* The interrupted code finds EBX, ESI, EDI and EBP as it left them. */
static inline void abi_expect_resumed_regs(const struct abi_spin *s, const char *what)
{
	static const char *const name[4] = { "EBX", "ESI", "EDI", "EBP" };
	int i;

	for (i = 0; i < 4; i++)
		pc_check(s->out_reg[i] == s->in_reg[i],
		    "%s: %s was 0x%08x when interrupted and 0x%08x when resumed", what, name[i],
		    s->in_reg[i], s->out_reg[i]);
}

/* The interrupted code finds the x87 control word as it left it. */
static inline void abi_expect_resumed_cw(const struct abi_spin *s, const char *what)
{
	pc_check(s->cw_after == s->cw_before,
	    "%s: the x87 control word was 0x%04x when interrupted and 0x%04x when resumed", what,
	    s->cw_before, s->cw_after);
}

/* The interrupted code finds the MXCSR control bits as it left them. */
static inline void abi_expect_resumed_mxcsr(const struct abi_spin *s, const char *what)
{
	pc_check(((s->mxcsr_before ^ s->mxcsr_after) & ABI_MXCSR_CONTROL) == 0,
	    "%s: the MXCSR control bits were 0x%04x when interrupted and 0x%04x when resumed", what,
	    s->mxcsr_before & ABI_MXCSR_CONTROL, s->mxcsr_after & ABI_MXCSR_CONTROL);
}

/* The interrupted code finds the direction flag as it left it. */
static inline void abi_expect_resumed_df(const struct abi_spin *s, int df_before, const char *what)
{
	pc_check(((s->eflags_after & ABI_EFLAGS_DF) != 0) == (df_before != 0),
	    "%s: the direction flag was %s when interrupted and %s when resumed", what,
	    df_before ? "set" : "clear", df_before ? "clear" : "set");
}

/* ---------- checks on a recorded function entry ---------- */

/* (ESP + 4) is a multiple of 4 at every entry recorded. */
static inline void abi_expect_entry_mod4(const struct abi_entry_rec *r, const char *what)
{
	pc_check(r->count > 0, "%s: the function was never entered", what);
	pc_check((r->esp_or & 3u) == 0,
	    "%s: ESP + 4 was not a multiple of 4 at entry", what);
}

/* (ESP + 4) is a multiple of 16 at every entry recorded. */
static inline void abi_expect_entry_mod16(const struct abi_entry_rec *r, const char *what)
{
	pc_check(r->count > 0, "%s: the function was never entered", what);
	pc_check(r->esp_or == 0,
	    "%s: ESP + 4 was not a multiple of 16 at entry (low bits seen: 0x%x, last ESP 0x%08x)",
	    what, r->esp_or, r->esp);
}

/* The direction flag was clear at every entry recorded. */
static inline void abi_expect_entry_df_clear(const struct abi_entry_rec *r, const char *what)
{
	pc_check(r->count > 0, "%s: the function was never entered", what);
	pc_check(r->df_or == 0, "%s: the direction flag was set at entry", what);
}

#endif /* ABI_ARCH_I386 */
#endif /* ABI_CHECK_H */
