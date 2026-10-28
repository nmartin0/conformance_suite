# tee copies standard input unchanged to standard output and to each named file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tee.001
. "$PC_CORE/pc.sh"
pc_begin XCU.tee.001
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'hello\nworld\n' >in1 || pc_fail "cannot create input"
printf 'no newline' >in2 || pc_fail "cannot create input"
pc_test
tee f1 f2 <in1 >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "tee f1 f2 gave status $rc"
for f in out f1 f2; do
	got=$(cat "$f"; printf x)
	[ "$got" = "hello${NL}world${NL}x" ] || pc_fail "$f is not a copy of the input"
done
tee <in2 >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "tee with no operand gave status $rc"
got=$(cat out; printf x)
[ "$got" = "no newlinex" ] || pc_fail "tee with no operand did not copy the input exactly"
tee f3 <in2 >out 2>/dev/null
got=$(cat f3; printf x)
[ "$got" = "no newlinex" ] || pc_fail "the file copy of an input with no final newline is not exact"
pc_pass
