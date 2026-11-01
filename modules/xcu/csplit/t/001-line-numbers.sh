# csplit with line-number operands writes each piece to xxNN files and prints each piece's size in bytes.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.csplit.001
# MUTANT    csplit.ln_plus1
# MUTANT    csplit.ln_minus1
# MUTANT    csplit.ln_relative
# MUTANT    csplit.ln_first_digit
# MUTANT    csplit.file_start1
# MUTANT    csplit.last_dropped
# MUTANT    csplit.extra_empty_file
# MUTANT    csplit.piece_nonl
# MUTANT    csplit.size_lines
# MUTANT    csplit.size_plus1
# MUTANT    csplit.size_nonl
# MUTANT    csplit.size_none
# MUTANT    csplit.size_stderr
# MUTANT    csplit.size_reversed
# MUTANT    csplit.size_last_missing
# MUTANT    csplit.size_named
# MUTANT    csplit.size_blank
# MUTANT    csplit.status1
# MUTANT    csplit.names_reversed
# MUTANT    csplit.boundary_lost
# MUTANT    csplit.input_limit
# MUTANT    csplit.long_line_cut
. "$PC_CORE/pc.sh"
pc_begin XCU.csplit.001
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '%s\n' 1 2 3 4 5 6 7 8 9 10 >in10 || pc_fail "cannot create input"
pc_test
got=$(csplit in10 3 6 2>err </dev/null; printf 'rc=%s' "$?")
[ "$got" = "4${NL}6${NL}11${NL}rc=0" ] || pc_fail "the size lines are not 4, 6, 11"
got=$(cat xx00; printf x)
[ "$got" = "1${NL}2${NL}x" ] || pc_fail "xx00 does not hold lines 1 and 2"
got=$(cat xx01; printf x)
[ "$got" = "3${NL}4${NL}5${NL}x" ] || pc_fail "xx01 does not hold lines 3 to 5"
got=$(cat xx02; printf x)
[ "$got" = "6${NL}7${NL}8${NL}9${NL}10${NL}x" ] || pc_fail "xx02 does not hold lines 6 to 10"
[ -e xx03 ] && pc_fail "a fourth file was created"
[ -s err ] && pc_fail "a successful run wrote to standard error"
rm -f xx00 xx01 xx02
# a line number of two digits
printf '%s\n' 1 2 3 4 5 6 7 8 9 10 11 12 >in12
csplit -s in12 10 >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit 10 gave status $rc"
got=$(cat xx00; printf x)
[ "$got" = "1${NL}2${NL}3${NL}4${NL}5${NL}6${NL}7${NL}8${NL}9${NL}x" ] || pc_fail "xx00 does not hold lines 1 to 9 for the operand 10"
got=$(cat xx01; printf x)
[ "$got" = "10${NL}11${NL}12${NL}x" ] || pc_fail "xx01 does not hold lines 10 to 12 for the operand 10"
rm -f xx00 xx01
# a long input: the pieces together are the input, the first has 1499 lines
i=0
: >big
while [ "$i" -lt 3000 ]; do
	i=$((i + 1))
	printf 'line number %s\n' "$i"
done >big
got=$(csplit big 1500 2>/dev/null </dev/null; printf 'rc=%s' "$?")
n0=$(wc -c <xx00)
n1=$(wc -c <xx01)
[ "$got" = "$((n0))${NL}$((n1))${NL}rc=0" ] || pc_fail "the size lines for the long input are not the file sizes"
[ "$(wc -l <xx00)" -eq 1499 ] || pc_fail "xx00 of the long input does not have 1499 lines"
cat xx00 xx01 >joined
cmp joined big >/dev/null 2>&1 || pc_fail "the pieces of the long input do not add up to the input"
rm -f xx00 xx01 joined big
# a line as long as the input is allowed to be (at most 2047 bytes and the newline)
l=x
while [ "${#l}" -lt 2000 ]; do l=$l$l; done
l=$(printf '%s' "$l" | cut -c1-2000)
printf '%s\n%s\nlast\n' "$l" "$l" >longl
csplit -s longl 2 >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit 2 on a long line gave status $rc"
printf '%s\n' "$l" >longl.want
cmp xx00 longl.want >/dev/null 2>&1 || pc_fail "xx00 does not hold the long line unchanged"
pc_pass
