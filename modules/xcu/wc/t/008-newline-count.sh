# wc -l counts newline characters, so a last line without a newline is not counted.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.wc.008
. "$PC_CORE/pc.sh"
pc_begin XCU.wc.008
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a\nb' >f1 || pc_fail "cannot create input"
printf 'abc' >f2 || pc_fail "cannot create input"
printf '\n\n\n' >f3 || pc_fail "cannot create input"
: >f4 || pc_fail "cannot create input"
pc_test
out=$(wc -l f1 2>/dev/null </dev/null)
set -- $out
[ "$1" = 1 ] || pc_fail "'a<newline>b' does not hold one newline"
out=$(wc -l f2 2>/dev/null </dev/null)
set -- $out
[ "$1" = 0 ] || pc_fail "'abc' does not hold zero newlines"
out=$(wc -l f3 2>/dev/null </dev/null)
set -- $out
[ "$1" = 3 ] || pc_fail "three newlines were not counted"
out=$(wc f4 2>/dev/null </dev/null)
set -- $out
[ "$1 $2 $3 $4" = "0 0 0 f4" ] || pc_fail "an empty file does not give 0 0 0"
pc_pass
