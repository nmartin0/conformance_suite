# join -o writes exactly the listed fields; 0 is the join field; the list may be comma- or blank-separated.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.join.006
# MUTANT    join.o_ignored
# MUTANT    join.o_comma_only
# MUTANT    join.o_space_only
# MUTANT    join.o_zero_missing
# MUTANT    join.o_with_key
# MUTANT    join.o_sorted
# MUTANT    join.o_swap_files
# MUTANT    join.o_off_by_one
# MUTANT    join.o_dup_dropped
# MUTANT    join.o_absent_dropped
# MUTANT    join.o_unpaired_default
# MUTANT    join.o_zero_unpaired_empty
# MUTANT    join.o_sep_space
# MUTANT    join.o_single_digit
# MUTANT    join.o_zero_f1
#
# An absent field is empty but keeps its separator. The list is one
# argument, separated by commas, blanks or tabs (all blanks).
. "$PC_CORE/pc.sh"
pc_begin XCU.join.006
LC_ALL=POSIX
export LC_ALL
NL='
'
TAB=$(printf '\t')
printf 'a 1 2\nb 3 4\n' >f1 && printf 'a x y\nb u v\n' >f2 || pc_fail "cannot create input"
printf 'k a q\n' >g1 && printf 'a w\n' >g2 || pc_fail "cannot create input"
printf 'a:1:2\n' >t1 && printf 'a:x:y\n' >t2 || pc_fail "cannot create input"
printf 'a 1 2 3 4 5 6 7 8 9 10 11\n' >w1 && printf 'a x\n' >w2 || pc_fail "cannot create input"
printf 'a 1\nb 2\nc 3\n' >u1 && printf 'a x\nb y\nd z\n' >u2 || pc_fail "cannot create input"
pc_test
got=$(join -o 1.2,2.3,0 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1 y a${NL}3 v b${NL}rc=0" ] || pc_fail "-o 1.2,2.3,0 gave the wrong result (got: $(printf %s "$got" | tr '\n' ' '))"
got=$(join -o "2.2 1.3" f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "x 2${NL}u 4${NL}rc=0" ] || pc_fail "-o '2.2 1.3' gave the wrong result (got: $(printf %s "$got" | tr '\n' ' '))"
got=$(join -o "1.2${TAB}2.2" f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1 x${NL}3 u${NL}rc=0" ] || pc_fail "a tab-separated -o list gave the wrong result (got: $(printf %s "$got" | tr '\n' ' '))"
got=$(join -o 1.1 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}b${NL}rc=0" ] || pc_fail "-o 1.1 did not write only that field"
got=$(join -o 2.2,1.2 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "x 1${NL}u 3${NL}rc=0" ] || pc_fail "-o 2.2,1.2 did not keep the order of the list"
got=$(join -o 1.2,1.2 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1 1${NL}3 3${NL}rc=0" ] || pc_fail "-o 1.2,1.2 did not write the field twice"
got=$(join -o 1.2,2.5,1.1 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1  a${NL}3  b${NL}rc=0" ] || pc_fail "an absent field in -o was not an empty field (got: $(printf %s "$got" | tr '\n' ' '))"
got=$(join -1 2 -2 1 -o 1.1,0,2.2 g1 g2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "k a w${NL}rc=0" ] || pc_fail "-o 0 was not the join field when -1 2 is given"
got=$(join -t : -o 0,2.2,1.3 t1 t2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a:x:2${NL}rc=0" ] || pc_fail "-t : -o did not separate with the character"
got=$(join -o 1.12,1.2 w1 w2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "11 1${NL}rc=0" ] || pc_fail "-o 1.12 gave the wrong result"
got=$(join -a 1 -o 1.2,0 u1 u2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1 a${NL}2 b${NL}3 c${NL}rc=0" ] || pc_fail "-o did not apply to unpairable lines of file1 (got: $(printf %s "$got" | tr '\n' ' '))"
got=$(join -a 2 -o 0,2.2 u1 u2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a x${NL}b y${NL}d z${NL}rc=0" ] || pc_fail "-o 0 was not the join field of an unpairable line of file2 (got: $(printf %s "$got" | tr '\n' ' '))"
pc_pass
