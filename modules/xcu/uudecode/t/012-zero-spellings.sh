# uudecode accepts both spellings of the zero value
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uudecode.012
# MUTANT    uudecode.space_rejected
# MUTANT    uudecode.backtick_rejected
# MUTANT    uudecode.term_space_rejected
# MUTANT    uudecode.term_backtick_rejected
# MUTANT    uudecode.status1
# MUTANT    uudecode.drop_last
# In historical-format data a zero 6-bit value may be written as a space or
# as a backtick (the same holds for the line that ends the data); both
# spellings decode to the same bytes.
. "$PC_CORE/pc.sh"
pc_begin XCU.uudecode.012
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# bytes 0 0 0 'C' 'a' 't'
printf 'begin 644 z1.out\n&````0V%%T\n`\nend\n' >in1 || pc_unresolved "setup: cannot write input"
printf 'begin 644 z2.out\n&    0V%%T\n \nend\n' >in2 || pc_unresolved "setup: cannot write input"
printf '\000\000\000Cat' >exp || pc_unresolved "setup: cannot write expectation"
pc_test
for n in 1 2; do
	uudecode in$n >stdout 2>err
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "input $n: exit status $rc"
	[ -f z$n.out ] || pc_fail "input $n: z$n.out was not created"
	cmp -s exp z$n.out || pc_fail "input $n: decoded content is wrong"
done
pc_pass
