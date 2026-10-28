# With no file operand wc reads standard input and writes no name and no blank after the last count.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.wc.006
. "$PC_CORE/pc.sh"
pc_begin XCU.wc.006
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'one two\nthree\n\nfour five six\n' >f || pc_fail "cannot create input"
pc_test
wc <f >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "wc <f gave status $rc"
IFS= read -r line <out || pc_fail "no output"
case $line in
*[0-9]) ;;
*) pc_fail "the output line does not end with a count" ;;
esac
set -- $line
[ $# -eq 3 ] && [ "$1" = 4 ] && [ "$2" = 6 ] && [ "$3" = 29 ] || pc_fail "wc <f is not three counts 4 6 29"
n=0
while IFS= read -r line; do n=$((n + 1)); done <out
[ "$n" -eq 1 ] || pc_fail "more than one output line"
out=$(wc -l <f 2>/dev/null)
set -- $out
[ $# -eq 1 ] && [ "$1" = 4 ] || pc_fail "wc -l <f is not the single number 4"
pc_pass
