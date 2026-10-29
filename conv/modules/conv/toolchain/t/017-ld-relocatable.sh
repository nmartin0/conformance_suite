# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION CONV.toolchain.017
#
# ld -r combines objects into one relocatable object (type REL) that keeps
# unresolved references unresolved, holds the definitions of every input and
# can itself be linked.
. "$PC_CORE/pc.sh"
. "${PC_CORE%/core}/conv/lib/tc.sh"
tc_begin CONV.toolchain.017
tc_need_ld
printf 'int tcext(void);\nint tca(void) { return tcext(); }\n' >a.c
printf 'int tcb(void) { return 2; }\n' >b.c
printf 'int tcext(void) { return 3; }\nint tcstart(void) { return 0; }\n' >c.c
for f in a b c; do tc_cobj $f.c; done
pc_test
tc_do "$LD" -r -o ab.o a.o b.o || pc_fail "ld -r failed on an object with an unresolved reference: $(tc_out)"
tc_dump ab.o
[ "$(tc_type ab.o)" = REL ] || pc_fail "the output of ld -r has type $(tc_type ab.o), not relocatable"
for n in tca tcb; do
	l=$(tc_sym ab.o $n)
	[ -n "$l" ] && [ "$(tc_key "$l" shndx)" != UND ] || pc_fail "$n is not defined in the combined object"
done
l=$(tc_sym ab.o tcext)
[ -n "$l" ] && [ "$(tc_key "$l" shndx)" = UND ] || pc_fail "tcext should remain an undefined reference in the combined object"
tc_do "$LD" -e tcstart -o prog ab.o c.o || pc_fail "linking the combined object failed: $(tc_out)"
tc_dump prog
l=$(tc_sym prog tcext)
[ -n "$l" ] && [ "$(tc_key "$l" shndx)" != UND ] || pc_fail "tcext is unresolved after linking the combined object with its definition"
pc_pass
