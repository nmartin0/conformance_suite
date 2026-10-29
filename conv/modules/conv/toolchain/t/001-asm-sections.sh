# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION CONV.toolchain.001
#
# .text, .data and .bss select the code, initialised-data and zero-filled
# sections; the object records them with the types and attributes the
# object-file specification gives those names, and a label defined after
# the directive belongs to that section.
. "$PC_CORE/pc.sh"
. "${PC_CORE%/core}/conv/lib/tc.sh"
tc_begin CONV.toolchain.001
tc_need AS CC
cat >a.s <<'EOT'
	.text
tcode:	.byte 0
	.data
tdata:	.byte 1
	.bss
tbss:	.skip 4
EOT
tc_aobj a.s
tc_dump a.o
pc_test
tc_chk_sec a.o .text PROGBITS AX W
tc_chk_sec a.o .data PROGBITS AW X
tc_chk_sec a.o .bss NOBITS AW X
for p in tcode:.text tdata:.data tbss:.bss; do
	l=$(tc_sym a.o "${p%%:*}")
	[ -n "$l" ] || pc_fail "label ${p%%:*} missing from the symbol table"
	[ "$(tc_key "$l" sec)" = "${p#*:}" ] || pc_fail "label ${p%%:*} is in $(tc_key "$l" sec), not ${p#*:}"
done
pc_pass
