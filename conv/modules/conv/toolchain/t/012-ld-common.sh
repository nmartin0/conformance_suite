# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION CONV.toolchain.012
#
# Several common symbols of one name are merged into one symbol of the
# largest size, in zero-filled storage; a real definition of the name wins
# over common symbols.
. "$PC_CORE/pc.sh"
. "${PC_CORE%/core}/conv/lib/tc.sh"
tc_begin CONV.toolchain.012
tc_need_ld AS
printf '\t.comm tcc,8,8\n' >c1.s
printf '\t.comm tcc,24,8\n' >c2.s
printf '\t.comm tcc,16,8\n' >c3.s
cat >d.s <<'EOT'
	.data
	.globl tcc
tcc:	.skip 4
	.size tcc, 4
EOT
printf 'int tcstart(void) { return 0; }\n' >m.c
for f in c1 c2 c3 d; do tc_aobj $f.s; done
tc_cobj m.c
pc_test
count() { awk '$1 == "SYM" && $2 == ".symtab" && $4 == "tcc"' "$1.dump" | wc -l | tr -d ' '; }
for order in "c1.o c2.o c3.o" "c3.o c2.o c1.o" "c2.o c1.o"; do
	tc_do "$LD" -e tcstart -o out m.o $order || pc_fail "link of common symbols $order failed: $(tc_out)"
	tc_dump out
	[ "$(count out)" = 1 ] || pc_fail "linking $order left $(count out) symbols named tcc"
	l=$(tc_sym out tcc)
	[ "$(tc_key "$l" shndx)" != UND ] && [ "$(tc_key "$l" shndx)" != COM ] || pc_fail "tcc is not allocated after linking $order"
	[ "$(tc_key "$l" size)" -ge 24 ] || pc_fail "linking $order gave tcc size $(tc_key "$l" size), below the largest (24)"
	s=$(tc_key "$l" sec)
	t=$(tc_sec out "$s")
	set -- $t
	[ "$4" = NOBITS ] || pc_fail "tcc is in section $s of type $4, not zero-filled storage"
done
tc_do "$LD" -e tcstart -o out2 m.o c2.o d.o || pc_fail "link of a common symbol with a definition failed: $(tc_out)"
tc_dump out2
l=$(tc_sym out2 tcc)
[ "$(tc_key "$l" size)" = 4 ] || pc_fail "the definition did not win over the common symbol (size $(tc_key "$l" size))"
pc_pass
