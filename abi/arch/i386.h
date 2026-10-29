/*
 * i386.h - architecture primitives for the i386 System V ABI checks.
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * This is the only place (with abi/arch/i386_control.h) where inline
 * assembly is allowed. Everything here is a primitive: read or set a piece
 * of machine state, call a function with chosen register contents and
 * report what came back, or record the machine state at the entry of a
 * function. The judgement (what the ABI requires) is in abi/check.h and in
 * the tests, which are generic C.
 *
 * On any other architecture this header defines nothing but leaves
 * ABI_ARCH_I386 undefined, and each i386 test reports NOTINUSE.
 *
 * The assembly is written to be position independent (a test may be built
 * as a position independent executable) and for the GNU assembler's
 * AT&T syntax, which every compiler used here accepts.
 */
#ifndef ABI_ARCH_I386_H
#define ABI_ARCH_I386_H

#if defined(__i386__)
#define ABI_ARCH_I386 1

#include <stddef.h>
#include <string.h>

/* ---------- constants from the psABI tables ---------- */

/* x87 control word at process entry (RC nearest, PC extended, all masked). */
#define ABI_X87_CW_DEFAULT     0x037Fu
/* x87 control word fields: precision control and rounding control. */
#define ABI_X87_CW_PC_MASK     0x0300u
#define ABI_X87_CW_RC_MASK     0x0C00u
#define ABI_X87_CW_MASKS       0x003Fu   /* the six exception masks */
/* MXCSR at process entry (all exceptions masked, rounding nearest, no FZ/DAZ). */
#define ABI_MXCSR_DEFAULT      0x1F80u
/* MXCSR control bits: DAZ, the six masks, RC and FZ (not the status flags). */
#define ABI_MXCSR_CONTROL      0xFFC0u
/* Two non-default control values used to make "preserved" observable. */
#define ABI_X87_CW_ALT         0x077Fu   /* round down, extended precision */
#define ABI_MXCSR_ALT          0x3F80u   /* round down */
#define ABI_EFLAGS_DF          0x0400u

/* Sentinels loaded into the callee-saved registers. */
#define ABI_SENT_EBX 0x1BADB001u
#define ABI_SENT_ESI 0x2BADB002u
#define ABI_SENT_EDI 0x3BADB003u
#define ABI_SENT_EBP 0x4BADB004u

/* ---------- state readers and setters (C-callable, inline) ---------- */

static inline unsigned abi_eflags(void)
{
	unsigned f;
	__asm__ volatile("pushfl\n\tpopl %0" : "=r"(f) : : "memory");
	return f;
}

static inline unsigned abi_x87_cw(void)
{
	unsigned short w;
	__asm__ volatile("fnstcw %0" : "=m"(w));
	return w;
}

static inline void abi_x87_set_cw(unsigned w)
{
	unsigned short v = (unsigned short)w;
	__asm__ volatile("fldcw %0" : : "m"(v));
}

/* x87 status word (its low six bits are the exception flags). */
static inline unsigned abi_x87_sw(void)
{
	unsigned short w;
	__asm__ volatile("fnstsw %0" : "=m"(w));
	return w;
}

static inline unsigned abi_mxcsr(void)
{
	unsigned m;
	__asm__ volatile("stmxcsr %0" : "=m"(m));
	return m;
}

static inline void abi_set_mxcsr(unsigned m)
{
	__asm__ volatile("ldmxcsr %0" : : "m"(m));
}

/* Number of x87 registers whose tag says "not empty" (0 = x87 mode, empty). */
static inline unsigned abi_x87_used(void)
{
	unsigned char env[28];
	unsigned tw, n = 0, i;
	__asm__ volatile("fnstenv %0\n\tfldenv %0" : "=m"(env) : : "memory");
	tw = (unsigned)env[8] | ((unsigned)env[9] << 8);
	for (i = 0; i < 8; i++)
		if (((tw >> (2 * i)) & 3u) != 3u)
			n++;
	return n;
}

/* CPUID leaf 1: 1 if the processor has SSE (needed to read MXCSR). */
static inline int abi_cpu_has_sse(void)
{
	unsigned a = 1, b, c, d;
	/* EBX is the GOT register in position independent code: save it by hand. */
	__asm__ volatile("movl %%ebx,%%esi\n\tcpuid\n\txchgl %%ebx,%%esi"
	    : "+a"(a), "=S"(b), "=c"(c), "=d"(d) : : "cc");
	(void)b; (void)c;
	return (d >> 25) & 1u;
}

/* ---------- calling a function with chosen register contents ---------- */

#define ABI_MAXW 16     /* argument words abi_call_raw can pass */

#define ABI_CALL_SETFP 1u   /* load cw_set and mxcsr_set before the call */
#define ABI_CALL_NOSSE 2u   /* do not touch MXCSR (processor without SSE) */

struct abi_call {
	/* inputs */
	void *fn;                       /*   0 */
	unsigned nwords;                /*   4 */
	unsigned args[ABI_MAXW];        /*   8 */
	unsigned misalign;              /*  72 bytes of extra displacement of ESP at the call */
	unsigned flags;                 /*  76 */
	unsigned in_reg[4];             /*  80 EBX ESI EDI EBP loaded before the call */
	unsigned cw_set;                /*  96 */
	unsigned mxcsr_set;             /* 100 */
	/* results */
	unsigned out_reg[4];            /* 104 EBX ESI EDI EBP after the call */
	unsigned eax;                   /* 120 */
	unsigned edx;                   /* 124 */
	unsigned eflags_after;          /* 128 */
	unsigned cw_orig;               /* 132 */
	unsigned mxcsr_orig;            /* 136 */
	unsigned cw_before;             /* 140 control words just before the call */
	unsigned mxcsr_before;          /* 144 */
	unsigned cw_after;              /* 148 control words just after the call */
	unsigned mxcsr_after;           /* 152 */
	unsigned esp_before;            /* 156 ESP at the call instruction */
	unsigned esp_after;             /* 160 ESP right after the return */
	unsigned npop;                  /* 164 values found on the x87 stack and popped */
	unsigned char st0[16];          /* 168 the first of them, as an 80-bit value */
	unsigned char env_before[28];   /* 184 x87 environment before the call */
	unsigned char env_after[28];    /* 212 and right after it */
};

#define AO_FN 0
#define AO_NWORDS 4
#define AO_ARGS 8
#define AO_MISALIGN 72
#define AO_FLAGS 76
#define AO_IN 80
#define AO_CWSET 96
#define AO_MXSET 100
#define AO_OUT 104
#define AO_EAX 120
#define AO_EDX 124
#define AO_EFLAGS 128
#define AO_CWORIG 132
#define AO_MXORIG 136
#define AO_CWBEFORE 140
#define AO_MXBEFORE 144
#define AO_CWAFTER 148
#define AO_MXAFTER 152
#define AO_ESPB 156
#define AO_ESPA 160
#define AO_NPOP 164
#define AO_ST0 168
#define AO_ENVB 184
#define AO_ENVA 212

_Static_assert(offsetof(struct abi_call, fn) == AO_FN, "layout");
_Static_assert(offsetof(struct abi_call, nwords) == AO_NWORDS, "layout");
_Static_assert(offsetof(struct abi_call, args) == AO_ARGS, "layout");
_Static_assert(offsetof(struct abi_call, misalign) == AO_MISALIGN, "layout");
_Static_assert(offsetof(struct abi_call, flags) == AO_FLAGS, "layout");
_Static_assert(offsetof(struct abi_call, in_reg) == AO_IN, "layout");
_Static_assert(offsetof(struct abi_call, cw_set) == AO_CWSET, "layout");
_Static_assert(offsetof(struct abi_call, mxcsr_set) == AO_MXSET, "layout");
_Static_assert(offsetof(struct abi_call, out_reg) == AO_OUT, "layout");
_Static_assert(offsetof(struct abi_call, eax) == AO_EAX, "layout");
_Static_assert(offsetof(struct abi_call, edx) == AO_EDX, "layout");
_Static_assert(offsetof(struct abi_call, eflags_after) == AO_EFLAGS, "layout");
_Static_assert(offsetof(struct abi_call, cw_orig) == AO_CWORIG, "layout");
_Static_assert(offsetof(struct abi_call, mxcsr_orig) == AO_MXORIG, "layout");
_Static_assert(offsetof(struct abi_call, cw_before) == AO_CWBEFORE, "layout");
_Static_assert(offsetof(struct abi_call, mxcsr_before) == AO_MXBEFORE, "layout");
_Static_assert(offsetof(struct abi_call, cw_after) == AO_CWAFTER, "layout");
_Static_assert(offsetof(struct abi_call, mxcsr_after) == AO_MXAFTER, "layout");
_Static_assert(offsetof(struct abi_call, esp_before) == AO_ESPB, "layout");
_Static_assert(offsetof(struct abi_call, esp_after) == AO_ESPA, "layout");
_Static_assert(offsetof(struct abi_call, npop) == AO_NPOP, "layout");
_Static_assert(offsetof(struct abi_call, st0) == AO_ST0, "layout");
_Static_assert(offsetof(struct abi_call, env_before) == AO_ENVB, "layout");
_Static_assert(offsetof(struct abi_call, env_after) == AO_ENVA, "layout");

#define ABI_Q1(x) #x
#define ABI_Q(x) ABI_Q1(x)

/*
 * void abi_call_raw(struct abi_call *c)
 *
 * Copies c->nwords argument words to the stack so that ESP is a multiple of
 * 16 (plus c->misalign) at the call instruction, loads EAX-independent
 * sentinels into EBX/ESI/EDI/EBP from c->in_reg, clears DF, optionally loads
 * the x87 and MXCSR control values, calls c->fn and records what the
 * callee left: the registers, EAX, EDX, EFLAGS, the control words, ESP, and
 * the values it left on the x87 stack (popped here, the first kept in st0).
 * The caller's own registers and control words are restored before it
 * returns. A callee that pops its hidden-pointer argument (returns with
 * "ret $4") is handled: ESP after the call may be 4 higher.
 */
__asm__(
".pushsection .text\n"
".globl abi_call_raw\n"
".hidden abi_call_raw\n"
".type abi_call_raw,@function\n"
"abi_call_raw:\n"
"\tpushl %ebp\n"
"\tmovl %esp,%ebp\n"
"\tpushl %ebx\n"
"\tpushl %esi\n"
"\tpushl %edi\n"
"\tmovl 8(%ebp),%ecx\n"
"\tfnstcw " ABI_Q(AO_CWORIG) "(%ecx)\n"
"\ttestl $2," ABI_Q(AO_FLAGS) "(%ecx)\n"
"\tjnz 1f\n"
"\tstmxcsr " ABI_Q(AO_MXORIG) "(%ecx)\n"
"1:\n"
"\ttestl $1," ABI_Q(AO_FLAGS) "(%ecx)\n"
"\tjz 2f\n"
"\tfldcw " ABI_Q(AO_CWSET) "(%ecx)\n"
"\ttestl $2," ABI_Q(AO_FLAGS) "(%ecx)\n"
"\tjnz 2f\n"
"\tldmxcsr " ABI_Q(AO_MXSET) "(%ecx)\n"
"2:\n"
"\tfnstcw " ABI_Q(AO_CWBEFORE) "(%ecx)\n"
"\ttestl $2," ABI_Q(AO_FLAGS) "(%ecx)\n"
"\tjnz 3f\n"
"\tstmxcsr " ABI_Q(AO_MXBEFORE) "(%ecx)\n"
"3:\n"
"\tfnstenv " ABI_Q(AO_ENVB) "(%ecx)\n"
"\tfldenv " ABI_Q(AO_ENVB) "(%ecx)\n"
"\tmovl " ABI_Q(AO_NWORDS) "(%ecx),%eax\n"
"\tsubl $128,%esp\n"
"\tandl $-16,%esp\n"
"\tsubl " ABI_Q(AO_MISALIGN) "(%ecx),%esp\n"
"\tmovl %ecx,68(%esp)\n"
"\tmovl %ecx,72(%esp)\n"
"\tmovl %ebp,76(%esp)\n"
"\tmovl %ebp,80(%esp)\n"
"\tcld\n"
"\tleal " ABI_Q(AO_ARGS) "(%ecx),%esi\n"
"\tmovl %esp,%edi\n"
"\tmovl %eax,%ecx\n"
"\trep movsl\n"
"\tmovl 68(%esp),%ecx\n"
"\tmovl %esp," ABI_Q(AO_ESPB) "(%ecx)\n"
"\tmovl " ABI_Q(AO_FN) "(%ecx),%eax\n"
"\tmovl " ABI_Q(AO_IN) "(%ecx),%ebx\n"
"\tmovl " ABI_Q(AO_IN+4) "(%ecx),%esi\n"
"\tmovl " ABI_Q(AO_IN+8) "(%ecx),%edi\n"
"\tmovl " ABI_Q(AO_IN+12) "(%ecx),%ebp\n"
"\tcall *%eax\n"
/* the callee may or may not have popped one word: both slots hold c */
"\tmovl 68(%esp),%ecx\n"
"\tmovl %esp," ABI_Q(AO_ESPA) "(%ecx)\n"
"\tmovl %eax," ABI_Q(AO_EAX) "(%ecx)\n"
"\tmovl %edx," ABI_Q(AO_EDX) "(%ecx)\n"
"\tmovl %ebx," ABI_Q(AO_OUT) "(%ecx)\n"
"\tmovl %esi," ABI_Q(AO_OUT+4) "(%ecx)\n"
"\tmovl %edi," ABI_Q(AO_OUT+8) "(%ecx)\n"
"\tmovl %ebp," ABI_Q(AO_OUT+12) "(%ecx)\n"
"\tpushfl\n"
"\tpopl " ABI_Q(AO_EFLAGS) "(%ecx)\n"
"\tcld\n"
"\tfnstcw " ABI_Q(AO_CWAFTER) "(%ecx)\n"
"\ttestl $2," ABI_Q(AO_FLAGS) "(%ecx)\n"
"\tjnz 4f\n"
"\tstmxcsr " ABI_Q(AO_MXAFTER) "(%ecx)\n"
"4:\n"
"\tfnstenv " ABI_Q(AO_ENVA) "(%ecx)\n"
"\tfldenv " ABI_Q(AO_ENVA) "(%ecx)\n"
"\txorl %edx,%edx\n"
"5:\n"
"\tfxam\n"
"\tfnstsw %ax\n"
"\tandl $0x4500,%eax\n"
"\tcmpl $0x4100,%eax\n"          /* C3 and C0 set, C2 clear: empty */
"\tje 8f\n"
"\ttestl %edx,%edx\n"
"\tjnz 6f\n"
"\tfstpt " ABI_Q(AO_ST0) "(%ecx)\n"
"\tjmp 7f\n"
"6:\n"
"\tfstp %st(0)\n"
"7:\n"
"\tincl %edx\n"
"\tcmpl $8,%edx\n"
"\tjb 5b\n"
"8:\n"
"\tmovl %edx," ABI_Q(AO_NPOP) "(%ecx)\n"
"\tfldcw " ABI_Q(AO_CWORIG) "(%ecx)\n"
"\ttestl $2," ABI_Q(AO_FLAGS) "(%ecx)\n"
"\tjnz 9f\n"
"\tldmxcsr " ABI_Q(AO_MXORIG) "(%ecx)\n"
"9:\n"
"\tmovl 76(%esp),%ebp\n"
"\tleal -12(%ebp),%esp\n"
"\tpopl %edi\n"
"\tpopl %esi\n"
"\tpopl %ebx\n"
"\tpopl %ebp\n"
"\tret\n"
".size abi_call_raw,.-abi_call_raw\n"
".popsection\n");

void abi_call_raw(struct abi_call *c);

/* Prepare a call: zeroed record, the usual sentinels, no arguments. */
static inline void abi_call_init(struct abi_call *c, void *fn)
{
	memset(c, 0, sizeof *c);
	c->fn = fn;
	c->in_reg[0] = ABI_SENT_EBX;
	c->in_reg[1] = ABI_SENT_ESI;
	c->in_reg[2] = ABI_SENT_EDI;
	c->in_reg[3] = ABI_SENT_EBP;
	if (!abi_cpu_has_sse())
		c->flags |= ABI_CALL_NOSSE;
}

static inline void abi_call_arg(struct abi_call *c, unsigned w)
{
	if (c->nwords < ABI_MAXW)
		c->args[c->nwords++] = w;
}

/* A double or long double is passed as 2 or 3 words, low word first. */
static inline void abi_call_arg_double(struct abi_call *c, double d)
{
	unsigned w[2];
	memcpy(w, &d, sizeof w);
	abi_call_arg(c, w[0]);
	abi_call_arg(c, w[1]);
}

static inline void abi_call_arg_ldouble(struct abi_call *c, long double d)
{
	unsigned w[3] = { 0, 0, 0 };
	memcpy(w, &d, 10);
	abi_call_arg(c, w[0]);
	abi_call_arg(c, w[1]);
	abi_call_arg(c, w[2]);
}

static inline void abi_call(struct abi_call *c)
{
	abi_call_raw(c);
}

/* The value a function left in st0, as a long double (valid if npop > 0). */
static inline long double abi_call_st0(const struct abi_call *c)
{
	long double v = 0;
	memcpy(&v, c->st0, 10);
	return v;
}

/* ---------- running code while a signal arrives ---------- */

struct abi_spin {
	unsigned in_reg[4];     /*  0 */
	unsigned cw_set;        /* 16 */
	unsigned mxcsr_set;     /* 20 */
	unsigned out_reg[4];    /* 24 */
	unsigned eflags_after;  /* 40 */
	unsigned cw_before;     /* 44 */
	unsigned mxcsr_before;  /* 48 */
	unsigned cw_after;      /* 52 */
	unsigned mxcsr_after;   /* 56 */
	unsigned cw_orig;       /* 60 */
	unsigned mxcsr_orig;    /* 64 */
	unsigned fp;            /* 68 */
	volatile unsigned ready;/* 72 set once the sentinels are loaded and the loop starts */
	unsigned df;            /* 76 non-zero: run the loop with DF set */
	unsigned misalign;      /* 80 bytes by which ESP is lowered while the loop runs */
};

#define SO_IN 0
#define SO_CWSET 16
#define SO_MXSET 20
#define SO_OUT 24
#define SO_EFLAGS 40
#define SO_CWBEFORE 44
#define SO_MXBEFORE 48
#define SO_CWAFTER 52
#define SO_MXAFTER 56
#define SO_CWORIG 60
#define SO_MXORIG 64
#define SO_FP 68
#define SO_READY 72
#define SO_DF 76
#define SO_MISALIGN 80

_Static_assert(offsetof(struct abi_spin, in_reg) == SO_IN, "layout");
_Static_assert(offsetof(struct abi_spin, cw_set) == SO_CWSET, "layout");
_Static_assert(offsetof(struct abi_spin, out_reg) == SO_OUT, "layout");
_Static_assert(offsetof(struct abi_spin, eflags_after) == SO_EFLAGS, "layout");
_Static_assert(offsetof(struct abi_spin, cw_after) == SO_CWAFTER, "layout");
_Static_assert(offsetof(struct abi_spin, mxcsr_orig) == SO_MXORIG, "layout");
_Static_assert(offsetof(struct abi_spin, fp) == SO_FP, "layout");
_Static_assert(offsetof(struct abi_spin, ready) == SO_READY, "layout");
_Static_assert(offsetof(struct abi_spin, df) == SO_DF, "layout");
_Static_assert(offsetof(struct abi_spin, misalign) == SO_MISALIGN, "layout");

/*
 * void abi_spin(volatile int *flag, struct abi_spin *s)
 *
 * Loads the sentinels, the control values and (if s->df) DF into the
 * machine, sets s->ready and loops until *flag becomes non-zero, then
 * records the registers and control words as they are at that point and
 * restores the caller's own state. The loop touches no register except ECX
 * and EDX, so whatever a signal handler and the return from it do to the
 * rest is visible afterwards.
 */
__asm__(
".pushsection .text\n"
".globl abi_spin\n"
".hidden abi_spin\n"
".type abi_spin,@function\n"
"abi_spin:\n"
"\tpushl %ebp\n"
"\tmovl %esp,%ebp\n"
"\tpushl %ebx\n"
"\tpushl %esi\n"
"\tpushl %edi\n"
"\tmovl 8(%ebp),%edx\n"
"\tmovl 12(%ebp),%ecx\n"
"\tfnstcw " ABI_Q(SO_CWORIG) "(%ecx)\n"
"\tstmxcsr " ABI_Q(SO_MXORIG) "(%ecx)\n"
"\tfldcw " ABI_Q(SO_CWSET) "(%ecx)\n"
"\tldmxcsr " ABI_Q(SO_MXSET) "(%ecx)\n"
"\tfnstcw " ABI_Q(SO_CWBEFORE) "(%ecx)\n"
"\tstmxcsr " ABI_Q(SO_MXBEFORE) "(%ecx)\n"
"\tmovl %ebp," ABI_Q(SO_FP) "(%ecx)\n"
"\tsubl " ABI_Q(SO_MISALIGN) "(%ecx),%esp\n"
"\tmovl " ABI_Q(SO_IN) "(%ecx),%ebx\n"
"\tmovl " ABI_Q(SO_IN+4) "(%ecx),%esi\n"
"\tmovl " ABI_Q(SO_IN+8) "(%ecx),%edi\n"
"\tmovl " ABI_Q(SO_IN+12) "(%ecx),%ebp\n"
"\tcmpl $0," ABI_Q(SO_DF) "(%ecx)\n"
"\tje 1f\n"
"\tstd\n"
"1:\n"
"\tmovl $1," ABI_Q(SO_READY) "(%ecx)\n"
"2:\n"
"\tcmpl $0,(%edx)\n"
"\tje 2b\n"
"\tmovl %ebx," ABI_Q(SO_OUT) "(%ecx)\n"
"\tmovl %esi," ABI_Q(SO_OUT+4) "(%ecx)\n"
"\tmovl %edi," ABI_Q(SO_OUT+8) "(%ecx)\n"
"\tmovl %ebp," ABI_Q(SO_OUT+12) "(%ecx)\n"
"\tpushfl\n"
"\tpopl " ABI_Q(SO_EFLAGS) "(%ecx)\n"
"\tcld\n"
"\tfnstcw " ABI_Q(SO_CWAFTER) "(%ecx)\n"
"\tstmxcsr " ABI_Q(SO_MXAFTER) "(%ecx)\n"
"\tfldcw " ABI_Q(SO_CWORIG) "(%ecx)\n"
"\tldmxcsr " ABI_Q(SO_MXORIG) "(%ecx)\n"
"\tmovl " ABI_Q(SO_FP) "(%ecx),%ebp\n"
"\tleal -12(%ebp),%esp\n"
"\tpopl %edi\n"
"\tpopl %esi\n"
"\tpopl %ebx\n"
"\tpopl %ebp\n"
"\tret\n"
".size abi_spin,.-abi_spin\n"
".popsection\n");

void abi_spin(volatile int *flag, struct abi_spin *s);

/* ---------- recording the machine state at a function's entry ---------- */

struct abi_entry_rec {
	unsigned esp;       /* ESP at the entry (pointing at the return address), last call */
	unsigned eflags;    /* EFLAGS at the entry, last call */
	unsigned cw;        /* x87 control word at the entry, last call */
	unsigned mxcsr;     /* MXCSR at the entry, last call */
	unsigned count;     /* number of entries recorded */
	unsigned esp_or;    /* OR over all entries of (ESP + 4) & 15 */
	unsigned df_or;     /* OR over all entries of the DF bit of EFLAGS */
};

/*
 * ABI_ENTRY_TRAMPOLINE(name, target): defines the global function `name`
 * which records the machine state as it is at its entry in the object
 * `name_rec` (a struct abi_entry_rec) and then jumps to `target`, so the
 * target runs exactly as if it had been entered directly. The caller of
 * `name` sees an ordinary function with target's prototype. Use it at file
 * scope after declaring `target` (hidden visibility is added here) and
 * `name` with the prototype to call it by. It saves and restores EAX, ECX
 * and EFLAGS, which are the only registers it uses.
 */
#define ABI_ENTRY_TRAMPOLINE(name, target)                                   \
	extern struct abi_entry_rec name##_rec                               \
	    __attribute__((visibility("hidden")));                           \
	__asm__(                                                             \
	".pushsection .bss\n"                                                \
	".balign 4\n"                                                        \
	".globl " #name "_rec\n"                                             \
	".hidden " #name "_rec\n"                                            \
	".type " #name "_rec,@object\n"                                      \
	".size " #name "_rec,32\n"                                           \
	#name "_rec:\n\t.space 32\n"                                         \
	".popsection\n"                                                      \
	".pushsection .text\n"                                               \
	".globl " #name "\n"                                                 \
	".hidden " #name "\n"                                                \
	".type " #name ",@function\n"                                        \
	#name ":\n"                                                          \
	"\tpushl %eax\n"                                                     \
	"\tpushl %ecx\n"                                                     \
	"\tpushfl\n"                                                         \
	"\tleal 12(%esp),%eax\n"            /* ESP at entry */               \
	"\tcall .Lpc_" #name "\n"                                            \
	".Lpc_" #name ":\n"                                                  \
	"\tpopl %ecx\n"                                                      \
	"\taddl $_GLOBAL_OFFSET_TABLE_+(.-.Lpc_" #name "),%ecx\n"            \
	"\tmovl %eax," #name "_rec@GOTOFF(%ecx)\n"                           \
	"\tmovl (%esp),%eax\n"                                               \
	"\tmovl %eax," #name "_rec@GOTOFF+4(%ecx)\n"                         \
	"\tandl $0x400,%eax\n"                                               \
	"\torl %eax," #name "_rec@GOTOFF+24(%ecx)\n"                         \
	"\tleal 16(%esp),%eax\n"            /* ESP + 4 at entry */           \
	"\tandl $15,%eax\n"                                                  \
	"\torl %eax," #name "_rec@GOTOFF+20(%ecx)\n"                         \
	"\tfnstcw " #name "_rec@GOTOFF+8(%ecx)\n"                            \
	"\tstmxcsr " #name "_rec@GOTOFF+12(%ecx)\n"                          \
	"\tincl " #name "_rec@GOTOFF+16(%ecx)\n"                             \
	"\tpopfl\n"                                                          \
	"\tpopl %ecx\n"                                                      \
	"\tpopl %eax\n"                                                      \
	"\tjmp " #target "\n"                                                \
	".size " #name ",.-" #name "\n"                                      \
	".popsection\n")

/* Attributes for a function that is the target of a trampoline (or called only from assembly). */
#define ABI_TARGET __attribute__((used, noinline, visibility("hidden")))

/* The recorded values the checks look at. */
static inline unsigned abi_rec_sp_plus4_mod16(const struct abi_entry_rec *r)
{
	return r->esp_or & 15u;  /* OR of (ESP+4) mod 16 over all entries */
}

/*
 * ABI_CLOBBERING_HANDLER(name, target): defines `void name(int sig)`, for use
 * as a signal handler. It calls the C function `void target(int)` (declare
 * it with ABI_TARGET) and then, before returning, leaves the machine in a
 * state a returning function must not leave: EBX, ESI, EDI, EBP, ECX and EDX
 * changed, DF set, the x87 control word and the MXCSR rounding changed. A
 * system that resumes the interrupted code by restoring what the signal
 * frame saved shows none of it afterwards; one that does not, does.
 */
#define ABI_CLOBBERING_HANDLER(name, target)                                 \
	__asm__(".pushsection .text\n.globl " #name "\n.hidden " #name       \
	    "\n.type " #name ",@function\n" #name ":\n"                      \
	    "\tsubl $12,%esp\n"                                              \
	    "\tmovl 16(%esp),%eax\n\tmovl %eax,(%esp)\n"                     \
	    "\tcall " #target "\n"                                           \
	    "\taddl $12,%esp\n"                                              \
	    "\tpushl $0x0B7F\n\tfldcw (%esp)\n\taddl $4,%esp\n"             \
	    "\tpushl $0x5F80\n\tldmxcsr (%esp)\n\taddl $4,%esp\n"           \
	    "\tmovl $0xBAD0BAD1,%ebx\n\tmovl $0xBAD0BAD2,%esi\n"            \
	    "\tmovl $0xBAD0BAD3,%edi\n\tmovl $0xBAD0BAD4,%ebp\n"            \
	    "\tmovl $0xBAD0BAD5,%ecx\n\tmovl $0xBAD0BAD6,%edx\n"            \
	    "\tstd\n\tret\n"                                                 \
	    ".size " #name ",.-" #name "\n.popsection\n")

/* ---------- small assembly functions for the tests ---------- */

/* Defines `unsigned char/short/int name(void)` whose EAX is exactly val. */
#define ABI_ASM_RET_EAX(name, val)                                           \
	__asm__(".pushsection .text\n.globl " #name "\n.hidden " #name       \
	    "\n.type " #name ",@function\n" #name ":\n\tmovl $" #val         \
	    ",%eax\n\tret\n.size " #name ",.-" #name "\n.popsection\n")

/*
 * Defines `void name(unsigned *out, ...)`: copies the 16 words that follow
 * its first argument to out[0..15] and stores ESP+4 of its entry in out[16].
 * Used to see where a caller put its arguments.
 */
#define ABI_ASM_CAPTURE_ARGS(name)                                           \
	__asm__(".pushsection .text\n.globl " #name "\n.hidden " #name       \
	    "\n.type " #name ",@function\n" #name ":\n"                      \
	    "\tpushl %esi\n\tpushl %edi\n"                                   \
	    "\tmovl 12(%esp),%edi\n"                                         \
	    "\tleal 12(%esp),%eax\n\tmovl %eax,64(%edi)\n"                    \
	    "\tleal 16(%esp),%esi\n"                                         \
	    "\tmovl $16,%ecx\n\tcld\n\trep movsl\n"                          \
	    "\tpopl %edi\n\tpopl %esi\n\tret\n"                              \
	    ".size " #name ",.-" #name "\n.popsection\n")

#endif /* __i386__ */
#endif /* ABI_ARCH_I386_H */
