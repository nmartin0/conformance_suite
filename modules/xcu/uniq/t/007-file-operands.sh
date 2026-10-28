# uniq reads input_file (or standard input for no operand or -) and writes output_file when one is given, leaving standard output empty.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uniq.007
. "$PC_CORE/pc.sh"
pc_begin XCU.uniq.007
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a\na\nb\n' >in || pc_fail "cannot create input"
pc_test
uniq in res >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "uniq in res gave status $rc"
[ -s out ] && pc_fail "standard output was written although output_file was given"
got=$(cat res; printf x)
[ "$got" = "a${NL}b${NL}x" ] || pc_fail "output_file does not hold the result"
uniq - res2 <in >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "uniq - res2 gave status $rc"
[ -s out ] && pc_fail "standard output was written although output_file was given with -"
got=$(cat res2; printf x)
[ "$got" = "a${NL}b${NL}x" ] || pc_fail "uniq - res2 did not read standard input"
pc_pass
