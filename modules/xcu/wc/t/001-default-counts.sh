# By default wc writes the number of newlines, words and bytes, then the file name.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.wc.001
# MUTANT    wc.swap_lw
# MUTANT    wc.default_no_words
# MUTANT    wc.default_no_lines
# MUTANT    wc.default_no_bytes
# MUTANT    wc.l_off
# MUTANT    wc.w_off
# MUTANT    wc.c_off
# MUTANT    wc.name_missing
# MUTANT    wc.name_first
# MUTANT    wc.name_altered
# MUTANT    wc.comma_sep
# MUTANT    wc.no_newline
# MUTANT    wc.out_stderr
# MUTANT    wc.status1
# MUTANT    wc.wrap16
# MUTANT    wc.cap6
# MUTANT    wc.total_single
# VARIANT   wc.padded
. "$PC_CORE/pc.sh"
pc_begin XCU.wc.001
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'one two\nthree\n\nfour five six\n' >f || pc_fail "cannot create input"
awk 'BEGIN { for (i = 0; i < 400000; i++) print "a b" }' >big || pc_fail "cannot create input"
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
# the entry is one line ended by a newline, written to standard output
wc f >out 2>err </dev/null
[ -s out ] || pc_fail "nothing was written to standard output"
set -- $(tail -c 1 out | od -An -tx1)
[ "$*" = 0a ] || pc_fail "the entry does not end with a newline"
# counts beyond sixteen bits and beyond six digits
out=$(wc big 2>/dev/null </dev/null)
set -- $out
[ $# -eq 4 ] && [ "$1" = 400000 ] && [ "$2" = 800000 ] && [ "$3" = 1600000 ] && [ "$4" = big ] || pc_fail "wc big is not '400000 800000 1600000 big'"
pc_pass
