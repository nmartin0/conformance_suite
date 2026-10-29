/*
 * i386_control.h - deliberately wrong routines for the ABI control.
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * NOT part of the checks. abi/control.sh runs the checks of abi/check.h
 * against these routines (and against conforming twins of them) to show
 * that a check really fails when the ABI is broken. Each "bad" routine
 * breaks exactly one rule; each "good" one is its conforming twin.
 *
 * Inline assembly is allowed here as in abi/arch/i386.h.
 */
#ifndef ABI_ARCH_I386_CONTROL_H
#define ABI_ARCH_I386_CONTROL_H

#include "i386.h"

#ifdef ABI_ARCH_I386

#define ABI_CTRL(name, ...)                                                  \
	__asm__(".pushsection .text\n.globl " #name "\n.hidden " #name       \
	    "\n.type " #name ",@function\n" #name ":\n" __VA_ARGS__          \
	    ".size " #name ",.-" #name "\n.popsection\n")

/* returns nothing, changes nothing */
ABI_CTRL(ctl_good_nop, "\tret\n");
/* clobbers a callee-saved register */
ABI_CTRL(ctl_bad_ebx, "\tmovl $0xDEAD0001,%ebx\n\tret\n");
ABI_CTRL(ctl_bad_esi, "\tmovl $0xDEAD0002,%esi\n\tret\n");
ABI_CTRL(ctl_bad_edi, "\tmovl $0xDEAD0003,%edi\n\tret\n");
ABI_CTRL(ctl_bad_ebp, "\tmovl $0xDEAD0004,%ebp\n\tret\n");
/* returns with the direction flag set */
ABI_CTRL(ctl_bad_df, "\tstd\n\tret\n");
/* changes the x87 control word (rounding) and does not restore it */
ABI_CTRL(ctl_bad_cw, "\tsubl $4,%esp\n\tfnstcw (%esp)\n\torw $0x0C00,(%esp)\n\tfldcw (%esp)\n\taddl $4,%esp\n\tret\n");
/* changes the MXCSR control bits (rounding) and does not restore them */
ABI_CTRL(ctl_bad_mxcsr, "\tsubl $4,%esp\n\tstmxcsr (%esp)\n\torl $0x6000,(%esp)\n\tldmxcsr (%esp)\n\taddl $4,%esp\n\tret\n");
/* an integer function that leaves a value on the x87 register stack */
ABI_CTRL(ctl_bad_x87_leak, "\tfld1\n\tret\n");
/* returns a double correctly: 1.0 in st0 */
ABI_CTRL(ctl_good_fp1, "\tfld1\n\tret\n");
/* "returns a double" without putting anything in st0 */
ABI_CTRL(ctl_bad_fp_none, "\tret\n");
/* "returns a double" leaving two values */
ABI_CTRL(ctl_bad_fp_two, "\tfld1\n\tfld1\n\tret\n");
/* returns a structure correctly: writes through the hidden pointer, returns it in EAX, pops it */
ABI_CTRL(ctl_good_struct, "\tmovl 4(%esp),%eax\n\tmovl $7,(%eax)\n\tret $4\n");
/* returns a small structure in registers, ignoring the hidden pointer */
ABI_CTRL(ctl_bad_struct_regs, "\tmovl $1,%eax\n\tmovl $2,%edx\n\tret\n");
/* uses the hidden pointer and returns it, but does not pop it */
ABI_CTRL(ctl_bad_struct_nopop, "\tmovl 4(%esp),%eax\n\tmovl $7,(%eax)\n\tret\n");
/* a 64-bit return value, right and wrong */
ABI_CTRL(ctl_good_ll, "\tmovl $5,%eax\n\tmovl $1,%edx\n\tret\n");
ABI_CTRL(ctl_bad_ll, "\tmovl $5,%eax\n\txorl %edx,%edx\n\tret\n");
/*
 * Callers: void caller(void (*f)(void)). The good one calls f with ESP a
 * multiple of 16; one misaligns the stack by 4 first; one calls with DF set.
 */
ABI_CTRL(ctl_good_caller, "\tpushl %ebp\n\tmovl %esp,%ebp\n\tsubl $8,%esp\n\tcall *8(%ebp)\n\tleave\n\tret\n");
ABI_CTRL(ctl_bad_caller_misaligned, "\tpushl %ebp\n\tmovl %esp,%ebp\n\tsubl $4,%esp\n\tcall *8(%ebp)\n\tleave\n\tret\n");
ABI_CTRL(ctl_bad_caller_df, "\tpushl %ebp\n\tmovl %esp,%ebp\n\tsubl $8,%esp\n\tstd\n\tcall *8(%ebp)\n\tcld\n\tleave\n\tret\n");

/* A caller of a function that returns a value in st0: one pops it, one ignores it. */
ABI_CTRL(ctl_good_fpcaller, "\tpushl %ebp\n\tmovl %esp,%ebp\n\tsubl $8,%esp\n\tcall *8(%ebp)\n\tfstp %st(0)\n\tleave\n\tret\n");
ABI_CTRL(ctl_bad_fpcaller, "\tpushl %ebp\n\tmovl %esp,%ebp\n\tsubl $8,%esp\n\tcall *8(%ebp)\n\tleave\n\tret\n");

#endif /* ABI_ARCH_I386 */
#endif /* ABI_ARCH_I386_CONTROL_H */
