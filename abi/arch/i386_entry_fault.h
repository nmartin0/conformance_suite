/*
 * i386_entry_fault.h - planted faults for the entry-state stub, for the control.
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * NOT part of the checks. Included by check.h only when a test is built with
 * -DABI_ENTRY_FAULT=n (abi/control.sh does that); the assembly is run by
 * ABI_ENTRY_STATE_STUB before it records the entry state:
 *   1  EDX is clobbered with a value that is not a code address
 *   2  EDX points at data (the stub's own record), not at code
 *   3  ESP is 4 lower while it is recorded (restored before the jump)
 */
#ifndef ABI_ARCH_I386_ENTRY_FAULT_H
#define ABI_ARCH_I386_ENTRY_FAULT_H

#if ABI_ENTRY_FAULT == 1
#define ABI_ENTRY_PRE_ASM "\tmovl $1,%edx\n"
#elif ABI_ENTRY_FAULT == 2
#define ABI_ENTRY_PRE_ASM "\tmovl $abi_ent_seen,%edx\n"
#elif ABI_ENTRY_FAULT == 3
#define ABI_ENTRY_PRE_ASM "\tsubl $4,%esp\n"
#define ABI_ENTRY_POST_ASM "\taddl $4,%esp\n"
#else
#error "unknown ABI_ENTRY_FAULT"
#endif

#endif
