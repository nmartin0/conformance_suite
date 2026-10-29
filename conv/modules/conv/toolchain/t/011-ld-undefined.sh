# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION CONV.toolchain.011
#
# An undefined reference makes the link of an executable fail; the same
# object is accepted when the output is a shared object or a relocatable
# object.
. "$PC_CORE/pc.sh"
. "${PC_CORE%/core}/conv/lib/tc.sh"
tc_begin CONV.toolchain.011
tc_need_ld
printf 'int tcmissing(void);\nint tcstart(void) { return tcmissing(); }\n' >m.c
tc_cobj m.c
pc_test
if tc_do "$LD" -e tcstart -o exe m.o; then
	pc_fail "ld produced an executable although tcmissing is undefined"
fi
tc_do "$LD" -r -o rel.o m.o || pc_fail "ld -r rejected an undefined reference: $(tc_out)"
tc_do "$LD" -shared -o lib.so m.o || pc_fail "ld -shared rejected an undefined reference: $(tc_out)"
pc_pass
