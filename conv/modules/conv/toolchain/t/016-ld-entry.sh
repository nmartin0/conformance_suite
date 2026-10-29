# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION CONV.toolchain.016
#
# ld -e SYMBOL makes the entry address of the output the address of that
# symbol.
. "$PC_CORE/pc.sh"
. "${PC_CORE%/core}/conv/lib/tc.sh"
tc_begin CONV.toolchain.016
tc_need_ld
cat >m.c <<'EOT'
int tcfirst(void) { return 1; }
int tcsecond(void) { return 2; }
int tcthird(void) { return 3; }
EOT
tc_cobj m.c
pc_test
for e in tcsecond tcthird tcfirst; do
	tc_do "$LD" -e $e -o out m.o || pc_fail "ld -e $e failed: $(tc_out)"
	tc_dump out
	l=$(tc_sym out $e)
	[ -n "$l" ] || pc_fail "symbol $e missing from the output"
	want=$(tc_key "$l" value)
	got=$(sed -n '1s/.* entry=\(0x[0-9a-f]*\).*/\1/p' out.dump)
	[ "$got" = "$want" ] || pc_fail "with -e $e the entry address is $got, the address of $e is $want"
done
pc_pass
