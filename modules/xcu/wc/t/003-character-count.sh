# wc -m writes the number of characters in place of the byte count.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.wc.003
# MUTANT    wc.m_ignored
# MUTANT    wc.m_adds_c
# MUTANT    wc.m_as_lines
# MUTANT    wc.m_as_words
# MUTANT    wc.name_missing
# MUTANT    wc.l_off
# MUTANT    wc.status1
# VARIANT   wc.padded
. "$PC_CORE/pc.sh"
pc_begin XCU.wc.003
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'one two\nthree\n\nfour five six\n' >f || pc_fail "cannot create input"
pc_test
out=$(wc -m f 2>/dev/null </dev/null)
rc=$?
set -- $out
[ "$rc" -eq 0 ] && [ $# -eq 2 ] && [ "$1" = 29 ] && [ "$2" = f ] || pc_fail "wc -m f is not '29 f'"
out=$(wc -lwm f 2>/dev/null </dev/null)
rc=$?
set -- $out
[ "$rc" -eq 0 ] && [ $# -eq 4 ] && [ "$1" = 4 ] && [ "$2" = 6 ] && [ "$3" = 29 ] && [ "$4" = f ] || pc_fail "wc -lwm f is not '4 6 29 f'"
pc_pass
