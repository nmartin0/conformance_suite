# tail -r writes lines last first, all of them or the last number given by -n.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tail.004
# MUTANT    tail.r_ignored
# MUTANT    tail.r_n_ignored
# MUTANT    tail.r_n_more
# MUTANT    tail.r_n_less
# MUTANT    tail.r_default10
# MUTANT    tail.r_forward_n
# MUTANT    tail.r_first_n
# MUTANT    tail.r_drops_last
# MUTANT    tail.r_drops_first
# MUTANT    tail.r_blank_dropped
# MUTANT    tail.r_joined
# MUTANT    tail.r_large_cap
# MUTANT    tail.stdin_opts_ignored
# VARIANT   tail.byte_writes
. "$PC_CORE/pc.sh"
pc_begin XCU.tail.004
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '%s\n' 1 2 3 4 5 >in || pc_fail "cannot create input"
printf '%s\n' 1 2 3 4 5 6 7 8 9 10 11 12 13 >f13 || pc_fail "cannot create input"
printf 'a\n\n\nb\n\nc\n' >blank || pc_fail "cannot create input"
# a thousand short lines: under the smallest buffer an implementation may use
awk 'BEGIN { for (i = 1; i <= 1000; i++) print "line " i }' >big || pc_fail "cannot create input"
awk 'BEGIN { for (i = 1000; i >= 1; i--) print "line " i }' >bigrev || pc_fail "cannot create input"
pc_test
got=$(tail -r in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "5${NL}4${NL}3${NL}2${NL}1${NL}rc=0" ] || pc_fail "-r did not reverse all lines"
got=$(tail -r -n 2 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "5${NL}4${NL}rc=0" ] || pc_fail "-r -n 2 did not write the last two lines reversed"
got=$(tail -r -n 1 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "5${NL}rc=0" ] || pc_fail "-r -n 1 did not write the last line"
got=$(tail -r -n 5 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "5${NL}4${NL}3${NL}2${NL}1${NL}rc=0" ] || pc_fail "-r -n 5 on a five line file did not reverse it whole"
got=$(tail -r -n 20 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "5${NL}4${NL}3${NL}2${NL}1${NL}rc=0" ] || pc_fail "-r -n 20 on a five line file did not reverse it whole"
got=$(tail -r f13 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "13${NL}12${NL}11${NL}10${NL}9${NL}8${NL}7${NL}6${NL}5${NL}4${NL}3${NL}2${NL}1${NL}rc=0" ] || pc_fail "-r without -n did not write every line of a thirteen line file"
got=$(tail -r blank 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "c${NL}${NL}b${NL}${NL}${NL}a${NL}rc=0" ] || pc_fail "-r did not keep the empty lines"
got=$(cat f13 | tail -r -n 2 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "13${NL}12${NL}rc=0" ] || pc_fail "-r -n 2 on standard input from a pipe is wrong"
tail -r big >out 2>/dev/null </dev/null || pc_fail "-r on a long file failed"
cmp -s out bigrev || pc_fail "-r did not reverse all lines of a long file"
pc_pass
