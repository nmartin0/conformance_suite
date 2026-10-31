# wc counts words as runs of characters separated by white space, including tabs and newlines.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.wc.007
# MUTANT    wc.w_nl_joins
# MUTANT    wc.w_tab_joins
# MUTANT    wc.w_cr_joins
# MUTANT    wc.w_ff_joins
# MUTANT    wc.w_vt_joins
# MUTANT    wc.w_lines
# MUTANT    wc.w_empty_fields
# MUTANT    wc.w_punct_splits
# MUTANT    wc.w_last_dropped
# MUTANT    wc.w_off
# VARIANT   wc.padded
. "$PC_CORE/pc.sh"
pc_begin XCU.wc.007
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a  b\tc\nd\n\n  e  \n' >f1 || pc_fail "cannot create input"
printf '\n \t \n\n' >f2 || pc_fail "cannot create input"
printf 'word' >f3 || pc_fail "cannot create input"
printf 'a\rb\fc\vd\n' >f4 || pc_fail "cannot create input"
printf 'x-y,z.w\n' >f5 || pc_fail "cannot create input"
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
out=$(wc -w f4 2>/dev/null </dev/null)
set -- $out
[ "$1" = 4 ] || pc_fail "wc -w did not take carriage return, form feed and vertical tab as white space"
out=$(wc -w f5 2>/dev/null </dev/null)
set -- $out
[ "$1" = 1 ] || pc_fail "wc -w split a word at punctuation"
pc_pass
