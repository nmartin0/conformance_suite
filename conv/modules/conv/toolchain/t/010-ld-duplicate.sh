# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION CONV.toolchain.010
#
# Two strong definitions of the same global symbol in different objects make
# the link fail.
. "$PC_CORE/pc.sh"
. "${PC_CORE%/core}/conv/lib/tc.sh"
tc_begin CONV.toolchain.010
tc_need_ld
printf 'int tcstart(void) { return 0; }\nint tcdup(void) { return 1; }\n' >a.c
printf 'int tcdup(void) { return 2; }\n' >b.c
tc_cobj a.c
tc_cobj b.c
pc_test
tc_do "$LD" -e tcstart -o ok a.o || pc_fail "the control link (one definition) failed: $(tc_out)"
if tc_do "$LD" -e tcstart -o dup a.o b.o; then
	pc_fail "ld accepted two strong definitions of tcdup"
fi
if tc_do "$LD" -e tcstart -o dup2 b.o a.o; then
	pc_fail "ld accepted two strong definitions of tcdup (other order)"
fi
pc_pass
