# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION CONV.toolchain.006
#
# ld -o NAME writes the output file under that name; without -o the output
# is named a.out.
. "$PC_CORE/pc.sh"
. "${PC_CORE%/core}/conv/lib/tc.sh"
tc_begin CONV.toolchain.006
tc_need_ld
printf 'int tcstart(void) { return 0; }\n' >m.c
tc_cobj m.c
pc_test
tc_do "$LD" -e tcstart -o tcnamed.out m.o || pc_fail "ld -o failed: $(tc_out)"
[ -f tcnamed.out ] || pc_fail "ld -o tcnamed.out did not create tcnamed.out"
[ ! -e a.out ] || pc_fail "ld -o tcnamed.out also created a.out"
tc_dump tcnamed.out
case $(tc_type tcnamed.out) in EXEC | DYN) ;; *) pc_fail "the output is not an executable object (type $(tc_type tcnamed.out))" ;; esac
tc_do "$LD" -e tcstart m.o || pc_fail "ld without -o failed: $(tc_out)"
[ -f a.out ] || pc_fail "ld without -o did not create a.out"
pc_pass
