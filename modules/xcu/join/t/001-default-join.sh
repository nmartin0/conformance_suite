# join writes the join field, then the other fields of both files, separated by single spaces; runs of blanks are one separator.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.join.001
# MUTANT    join.sep_tab
# MUTANT    join.lead_blank_field
# MUTANT    join.blank_each
# MUTANT    join.space_only
# MUTANT    join.no_final_newline
# MUTANT    join.swap_files
# MUTANT    join.drop_key
# MUTANT    join.ignore_case
# MUTANT    join.prefix_key
# MUTANT    join.pair_unmatched
# MUTANT    join.status1
# MUTANT    join.to_stderr
# MUTANT    join.stop_after_first
# MUTANT    join.drop_last
# MUTANT    join.trunc_line
# MUTANT    join.cap_input
#
# Keys that differ only in case, or after the first character, must not
# pair. A long line and a long input are there for the fault that cuts or
# stops early (lines stay far below the minimum line length limit).
. "$PC_CORE/pc.sh"
pc_begin XCU.join.001
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a 1\nb 2\nc 3\n' >f1 && printf 'a x\nb y\nd z\n' >f2 || pc_fail "cannot create input"
printf '  a    1\nb\t2\n' >g1 && printf 'a x\nb   y\n' >g2 || pc_fail "cannot create input"
printf 'A 1\nab 2\nb 3\n' >h1 && printf 'A x\na y\nac z\nb w\n' >h2 || pc_fail "cannot create input"
awk 'BEGIN { s = sprintf("%600s", ""); t = s; gsub(/ /, "x", s); gsub(/ /, "y", t); print "k " s > "l1"; print "k " t > "l2"; print "k " s " " t > "lw" }' || pc_fail "cannot create input"
awk 'BEGIN { for (i = 1; i <= 3000; i++) { printf "%05d a\n", i > "m1"; printf "%05d b\n", i > "m2"; printf "%05d a b\n", i > "mw" } }' || pc_fail "cannot create input"
pc_test
got=$(join f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a 1 x${NL}b 2 y${NL}rc=0" ] || pc_fail "paired lines were not joined on the first field"
got=$(join g1 g2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a 1 x${NL}b 2 y${NL}rc=0" ] || pc_fail "leading blanks or runs of blanks were not treated as single separators"
got=$(join h1 h2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "A 1 x${NL}b 3 w${NL}rc=0" ] || pc_fail "join fields that differ in case or after the first byte were paired"
join l1 l2 >out 2>/dev/null </dev/null
rc=$?
[ -f out ] || pc_fail "no output was observed"
cmp -s out lw || pc_fail "a long line was not joined whole"
[ "$rc" -eq 0 ] || pc_fail "status $rc for a long line"
join m1 m2 >out 2>/dev/null </dev/null
rc=$?
[ -f out ] || pc_fail "no output was observed"
cmp -s out mw || pc_fail "a long input was not joined whole"
[ "$rc" -eq 0 ] || pc_fail "status $rc for a long input"
pc_pass
