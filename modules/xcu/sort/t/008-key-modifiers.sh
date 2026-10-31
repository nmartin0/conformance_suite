# Several -k keys apply in order; a type letter on a key applies to that key only; field.character positions restrict a key, and -u compares only the key.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sort.008
# MUTANT    sort.keys_reversed
# MUTANT    sort.keys_first_only
# MUTANT    sort.mod_ignored
# MUTANT    sort.mod_global_last
# MUTANT    sort.char_start_ignored
# MUTANT    sort.char_start_zero
# MUTANT    sort.char_end_ignored
# MUTANT    sort.char_end_excl
# MUTANT    sort.ec0_empty
# MUTANT    sort.swap_range
# MUTANT    sort.u_whole_line
# MUTANT    sort.u_first_key
# VARIANT   sort.u_last
. "$PC_CORE/pc.sh"
pc_begin XCU.sort.008
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'b 2\na 2\nc 1\nb 10\n' >in1 || pc_fail "cannot create input"
printf 'xb\nya\nzc\n' >in2 || pc_fail "cannot create input"
printf 'abz\nabc\nacb\n' >in3 || pc_fail "cannot create input"
printf 'xca\nybc\nzbb\n' >in7 || pc_fail "cannot create input"
printf 'b 1\na 2\n' >in4 || pc_fail "cannot create input"
printf 'c d\na b e\n' >in5 || pc_fail "cannot create input"
printf 'a 2 y\nb 2 y\nc 2 x\nd 1 z\n' >in6 || pc_fail "cannot create input"
pc_test
got=$(sort -k 2,2n -k 1,1r in1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "c 1${NL}b 2${NL}a 2${NL}b 10${NL}rc=0" ] || pc_fail "-k 2,2n -k 1,1r order is wrong"
got=$(sort -k 1.2,1.2 in2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "ya${NL}xb${NL}zc${NL}rc=0" ] || pc_fail "-k 1.2,1.2 order is wrong"
got=$(sort -u -k 1.1,1.2 in3 2>/dev/null </dev/null; printf 'rc=%s' "$?")
case $got in
"abz${NL}acb${NL}rc=0" | "abc${NL}acb${NL}rc=0") ;;
*) pc_fail "-u -k 1.1,1.2 did not compare only the first two characters" ;;
esac
got=$(sort -k 1.2,1.0 in7 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "zbb${NL}ybc${NL}xca${NL}rc=0" ] || pc_fail "a last character of zero did not mean the end of the field"
got=$(sort -k 2,1 -k 1,1r in4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "b 1${NL}a 2${NL}rc=0" ] || pc_fail "a key ending before its start was not empty"
got=$(sort -k 3 in5 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "c d${NL}a b e${NL}rc=0" ] || pc_fail "a key starting beyond the end of the line was not empty"
got=$(sort -u -k 2,2 -k 3,3 in6 2>/dev/null </dev/null; printf 'rc=%s' "$?")
case $got in
"d 1 z${NL}c 2 x${NL}a 2 y${NL}rc=0" | "d 1 z${NL}c 2 x${NL}b 2 y${NL}rc=0") ;;
*) pc_fail "-u with two keys did not compare both" ;;
esac
pc_pass
