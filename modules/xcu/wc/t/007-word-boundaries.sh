# wc counts words as runs of characters separated by white space, including tabs and newlines.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.wc.007
. "$PC_CORE/pc.sh"
pc_begin XCU.wc.007
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a  b\tc\nd\n\n  e  \n' >f1 || pc_fail "cannot create input"
printf '\n \t \n\n' >f2 || pc_fail "cannot create input"
printf 'word' >f3 || pc_fail "cannot create input"
pc_test
out=$(wc -w f1 2>/dev/null </dev/null)
set -- $out
[ "$1" = 5 ] || pc_fail "wc -w did not count five words"
out=$(wc -w f2 2>/dev/null </dev/null)
set -- $out
[ "$1" = 0 ] || pc_fail "wc -w counted words in white space only"
out=$(wc -w f3 2>/dev/null </dev/null)
set -- $out
[ "$1" = 1 ] || pc_fail "wc -w did not count the last word with no newline after it"
pc_pass
