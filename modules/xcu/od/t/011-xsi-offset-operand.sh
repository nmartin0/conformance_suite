# On XSI systems a final operand that is +offset (or a number after a file) is the octal byte offset at which to start, a trailing dot making it decimal.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.od.011
. "$PC_CORE/pc.sh"
pc_begin XCU.od.011
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abcdefgh' >f1 || pc_fail "cannot create input"
data_of() {
	data=
	while IFS= read -r line; do
		set -- $line
		[ $# -gt 1 ] || continue
		shift
		data="$data $*"
	done <"$1"
	set -- $data
	dataout=$*
}
pc_test
od -b f1 +2. >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "od -b f1 +2. gave status $rc"
data_of out
[ "$dataout" = "143 144 145 146 147 150" ] || pc_fail "+2. did not start at the third byte"
od -b f1 +4 >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "od -b f1 +4 gave status $rc"
data_of out
[ "$dataout" = "145 146 147 150" ] || pc_fail "+4 (octal) did not start at the fifth byte"
od -b f1 7 >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "a numeric last operand after a file was not taken as an offset"
data_of out
[ "$dataout" = "150" ] || pc_fail "the offset operand 7 (octal) did not start at the eighth byte"
pc_pass
