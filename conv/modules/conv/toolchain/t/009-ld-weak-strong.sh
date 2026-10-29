# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION CONV.toolchain.009
#
# When a symbol has a strong definition and a weak one, the strong one is
# used whatever the order of the objects; a weak reference that nothing
# defines is not an error.
. "$PC_CORE/pc.sh"
. "${PC_CORE%/core}/conv/lib/tc.sh"
tc_begin CONV.toolchain.009
tc_need_ld AS
cat >s.s <<'EOT'
	.data
	.globl tcv
tcv:	.skip 4
	.size tcv, 4
EOT
cat >w.s <<'EOT'
	.data
	.weak tcv
tcv:	.skip 8
	.size tcv, 8
EOT
cat >u.s <<'EOT'
	.text
	.globl tcstart
tcstart:	.byte 0
	.weak tcnodef
EOT
printf 'int tcstart(void) { return 0; }\n' >m.c
tc_aobj s.s
tc_aobj w.s
tc_aobj u.s
tc_cobj m.c
pc_test
for order in "s.o w.o" "w.o s.o"; do
	tc_do "$LD" -e tcstart -o out m.o $order || pc_fail "link of $order failed: $(tc_out)"
	tc_dump out
	l=$(tc_sym out tcv)
	[ -n "$l" ] || pc_fail "tcv missing after linking $order"
	[ "$(tc_key "$l" size)" = 4 ] || pc_fail "linking $order kept the weak definition (size $(tc_key "$l" size)), not the strong one (size 4)"
	[ "$(tc_key "$l" bind)" != WEAK ] || pc_fail "linking $order left tcv weak"
done
tc_do "$LD" -e tcstart -o out2 u.o || pc_fail "an undefined weak reference was an error: $(tc_out)"
tc_dump out2
l=$(tc_sym out2 tcnodef)
if [ -n "$l" ] && [ "$(tc_key "$l" shndx)" != UND ]; then
	[ "$(tc_key "$l" value)" = 0x0 ] || pc_fail "the unresolved weak symbol has value $(tc_key "$l" value), not zero"
fi
pc_pass
