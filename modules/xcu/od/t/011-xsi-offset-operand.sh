# On XSI systems a final operand that is +offset (or a number after a file) is the octal byte offset at which to start, a trailing dot making it decimal.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.od.011
# MUTANT    od.xo_ignored
# MUTANT    od.xo_octal_decimal
# MUTANT    od.xo_dot_octal
# MUTANT    od.xo_b_ignored
# MUTANT    od.xo_b_1024
# MUTANT    od.xo_always
# MUTANT    od.xo_three
# MUTANT    od.xo_num_single
# MUTANT    od.xo_plus_file
# MUTANT    od.xo_num_file
# VARIANT   od.A_default_d
# VARIANT   od.A_default_x
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
printf 'abcdefghijkl' >f12 || pc_fail "cannot create input"
# 512 a, 512 b, 76 c: the b multiplier is 512 bytes
awk 'BEGIN { for (i = 0; i < 512; i++) printf "a"; for (i = 0; i < 512; i++) printf "b"; for (i = 0; i < 76; i++) printf "c" }' >f1100 || pc_fail "cannot create input"
printf 'Z' >7 || pc_fail "cannot create input"
printf 'Y' >./+2 || pc_fail "cannot create input"
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
# octal by default, decimal with a dot: 10 is eight, 10. is ten
od -b f12 +10 >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "od -b f12 +10 gave status $rc"
data_of out
[ "$dataout" = "151 152 153 154" ] || pc_fail "+10 did not start at the ninth byte (octal)"
od -b f12 +10. >out 2>/dev/null </dev/null
data_of out
[ "$dataout" = "153 154" ] || pc_fail "+10. did not start at the eleventh byte (decimal)"
od -b f12 10 >out 2>/dev/null </dev/null
data_of out
[ "$dataout" = "151 152 153 154" ] || pc_fail "the operand 10 after a file did not start at the ninth byte"
od -b f12 10. >out 2>/dev/null </dev/null
data_of out
[ "$dataout" = "153 154" ] || pc_fail "the operand 10. after a file did not start at the eleventh byte"
# a b suffix counts blocks of 512 bytes
od -b f1100 +1b >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "od -b f1100 +1b gave status $rc"
data_of out
set -- $dataout
[ $# -eq 588 ] && [ "$1" = 142 ] || pc_fail "+1b did not start 512 bytes in"
od -b f1100 +2b >out 2>/dev/null </dev/null
data_of out
set -- $dataout
[ $# -eq 76 ] && [ "$1" = 143 ] || pc_fail "+2b did not start 1024 bytes in"
od -b f1100 2.b >out 2>/dev/null </dev/null
data_of out
set -- $dataout
[ $# -eq 76 ] && [ "$1" = 143 ] || pc_fail "2.b did not start 1024 bytes in"
# a + operand alone is an offset into standard input
od -b +2 <f12 >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "od -b +2 on standard input gave status $rc"
data_of out
[ "$dataout" = "143 144 145 146 147 150 151 152 153 154" ] || pc_fail "a lone +2 was not an offset into standard input"
# the operand is a file name when a file with that name is the only operand,
# when an option that rules the offset out is present, or with three operands
od -b 7 >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "od -b 7 (a file) gave status $rc"
data_of out
[ "$dataout" = "132" ] || pc_fail "a single numeric operand was not taken as a file name"
for opts in "-v" "-A d" "-j 0" "-N 100" "-t o1"; do
	od -b $opts f12 7 >out 2>/dev/null </dev/null
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "od -b $opts f12 7 gave status $rc"
	data_of out
	[ "$dataout" = "141 142 143 144 145 146 147 150 151 152 153 154 132" ] || pc_fail "with $opts the operand 7 was not a file name"
done
od -b f12 f12 7 >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "od -b f12 f12 7 gave status $rc"
data_of out
[ "$dataout" = "141 142 143 144 145 146 147 150 151 152 153 154 141 142 143 144 145 146 147 150 151 152 153 154 132" ] || pc_fail "with three operands the last was taken as an offset"
pc_pass
