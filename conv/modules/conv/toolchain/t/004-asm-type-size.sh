# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION CONV.toolchain.004
#
# .type marks a symbol as a function or a data object, .size sets the size
# recorded for it (a number or label arithmetic), and a symbol with neither
# has no type and size zero.
. "$PC_CORE/pc.sh"
. "${PC_CORE%/core}/conv/lib/tc.sh"
tc_begin CONV.toolchain.004
tc_need AS CC
# the type description has several documented spellings, one of which is
# not a comment character on a given processor
for fn in "@function:@object" "%function:%object" "STT_FUNC:STT_OBJECT" "#function:#object"; do
	f=${fn%%:*}
	o=${fn#*:}
	case $f in
	STT_*) tf=" $f" to=" $o" ;;
	*) tf=",$f" to=",$o" ;;
	esac
	cat >a.s <<EOT
	.text
	.globl tcf
	.type tcf${tf}
tcf:	.byte 0
	.byte 0
	.byte 0
	.size tcf, . - tcf
	.data
	.globl tco
	.type tco${to}
tco:	.skip 4
	.size tco, 4
tcn:	.byte 0
EOT
	if tc_do "$AS" -o a.o a.s; then
		ok=$f
		break
	fi
done
pc_test
[ -n "${ok:-}" ] || pc_fail "the assembler accepts .type in none of the documented spellings: $(tc_out)"
tc_dump a.o
l=$(tc_sym a.o tcf)
[ "$(tc_key "$l" type)" = FUNC ] || pc_fail "tcf has type $(tc_key "$l" type), not FUNC"
[ "$(tc_key "$l" size)" = 3 ] || pc_fail "tcf has size $(tc_key "$l" size), not 3"
l=$(tc_sym a.o tco)
[ "$(tc_key "$l" type)" = OBJECT ] || pc_fail "tco has type $(tc_key "$l" type), not OBJECT"
[ "$(tc_key "$l" size)" = 4 ] || pc_fail "tco has size $(tc_key "$l" size), not 4"
l=$(tc_sym a.o tcn)
[ "$(tc_key "$l" type)" = NOTYPE ] || pc_fail "tcn has type $(tc_key "$l" type), not NOTYPE"
[ "$(tc_key "$l" size)" = 0 ] || pc_fail "tcn has size $(tc_key "$l" size), not 0"
pc_pass
