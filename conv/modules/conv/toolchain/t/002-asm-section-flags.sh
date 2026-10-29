# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION CONV.toolchain.002
#
# .section NAME,"FLAGS",TYPE creates a section whose attributes follow the
# flag letters a (allocated), w (writable), x (executable) and whose type
# follows the type word (progbits, nobits).
. "$PC_CORE/pc.sh"
. "${PC_CORE%/core}/conv/lib/tc.sh"
tc_begin CONV.toolchain.002
tc_need AS CC
# The type word is introduced by a character that is a comment character on
# some processors; try the documented spellings.
for q in @ % '#'; do
	cat >a.s <<EOT
	.section .tcfoo,"a",${q}progbits
	.byte 0
	.section .tcbar,"aw",${q}progbits
	.byte 0
	.section .tcbaz,"ax",${q}progbits
	.byte 0
	.section .tczed,"aw",${q}nobits
	.skip 4
EOT
	if tc_do "$AS" -o a.o a.s; then
		ok=$q
		break
	fi
done
pc_test
[ -n "${ok:-}" ] || pc_fail "the assembler accepts .section with flags and a type in none of the documented spellings: $(tc_out)"
tc_dump a.o
tc_chk_sec a.o .tcfoo PROGBITS A WX
tc_chk_sec a.o .tcbar PROGBITS AW X
tc_chk_sec a.o .tcbaz PROGBITS AX W
tc_chk_sec a.o .tczed NOBITS AW X
pc_pass
