# uniq writes one copy of each run of identical adjacent lines and leaves non-adjacent repeats alone.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uniq.001
# MUTANT    uniq.nonadj
# MUTANT    uniq.never_collapse
# MUTANT    uniq.no_stdin
# MUTANT    uniq.no_newline
# MUTANT    uniq.first_char
# MUTANT    uniq.ignore_case
# MUTANT    uniq.drops_empty
# MUTANT    uniq.trailing_blank_ignored
# MUTANT    uniq.trunc1024
# MUTANT    uniq.status1
. "$PC_CORE/pc.sh"
pc_begin XCU.uniq.001
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a\na\nb\na\nc\nc\nc\nB\nb\nab\nac\nac \n\n\nx\n' >in || pc_fail "cannot create input"
# two long lines that differ only in their last byte, then one repeated
awk 'BEGIN { s = sprintf("%3000s", ""); gsub(/ /, "z", s); print s "1"; print s "2"; print s "2" }' >inlong || pc_fail "cannot create input"
pc_test
got=$(uniq in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
want="a${NL}b${NL}a${NL}c${NL}B${NL}b${NL}ab${NL}ac${NL}ac ${NL}${NL}x${NL}rc=0"
[ "$got" = "$want" ] || pc_fail "adjacent duplicates were not collapsed exactly"
got=$(uniq <in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "$want" ] || pc_fail "standard input was not read without operands"
uniq inlong >outlong 2>/dev/null </dev/null || pc_fail "uniq failed on long lines"
[ "$(wc -l <outlong)" -eq 2 ] || pc_fail "long lines that differ late were not told apart, or equal ones not collapsed"
pc_pass
