# By default wc writes the number of newlines, words and bytes, then the file name.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.wc.001
. "$PC_CORE/pc.sh"
pc_begin XCU.wc.001
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'one two\nthree\n\nfour five six\n' >f || pc_fail "cannot create input"
pc_test
out=$(wc f 2>/dev/null </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "wc f gave status $rc"
set -- $out
[ $# -eq 4 ] || pc_fail "output is not newlines, words, bytes and name"
[ "$1" = 4 ] || pc_fail "line count is not 4"
[ "$2" = 6 ] || pc_fail "word count is not 6"
[ "$3" = 29 ] || pc_fail "byte count is not 29"
[ "$4" = f ] || pc_fail "file name is not written after the counts"
pc_pass
