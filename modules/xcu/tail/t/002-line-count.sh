# tail -n number starts relative to the end of the file, or from the numberth line when the number has a plus sign.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tail.002
# MUTANT    tail.n_ignored
# MUTANT    tail.n_less
# MUTANT    tail.n_more
# MUTANT    tail.n_plus_off_late
# MUTANT    tail.n_plus_off_early
# MUTANT    tail.n_plus_as_minus
# MUTANT    tail.n_minus_as_plus
# MUTANT    tail.n_as_bytes
# MUTANT    tail.n_zero_default
# MUTANT    tail.n_first_digit
# MUTANT    tail.n_big_nothing
# MUTANT    tail.n_blank_dropped
# MUTANT    tail.large_first_block
# MUTANT    tail.attached_rejected
# VARIANT   tail.byte_writes
# VARIANT   tail.buffer_limit
. "$PC_CORE/pc.sh"
pc_begin XCU.tail.002
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '%s\n' 1 2 3 4 5 6 7 8 9 10 11 12 >f12 || pc_fail "cannot create input"
printf 'x\n\n\ny\n\n' >blank || pc_fail "cannot create input"
awk 'BEGIN { for (i = 1; i <= 20000; i++) print "line " i }' >big || pc_fail "cannot create input"
printf 'line 19998\nline 19999\nline 20000\n' >bigtail || pc_fail "cannot create input"
printf 'line 19999\nline 20000\n' >bigplus || pc_fail "cannot create input"
pc_test
got=$(tail -n 3 f12 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "10${NL}11${NL}12${NL}rc=0" ] || pc_fail "-n 3 did not write the last three lines"
got=$(tail -n -3 f12 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "10${NL}11${NL}12${NL}rc=0" ] || pc_fail "-n -3 did not write the last three lines"
got=$(tail -n +11 f12 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "11${NL}12${NL}rc=0" ] || pc_fail "-n +11 did not start at the eleventh line"
got=$(tail -n +1 f12 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}2${NL}3${NL}4${NL}5${NL}6${NL}7${NL}8${NL}9${NL}10${NL}11${NL}12${NL}rc=0" ] || pc_fail "-n +1 did not write the whole file"
got=$(tail -n +12 f12 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "12${NL}rc=0" ] || pc_fail "-n +12 did not start at the last line"
got=$(tail -n +13 f12 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "rc=0" ] || pc_fail "-n +13 on a twelve line file did not write nothing"
got=$(tail -n 1 f12 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "12${NL}rc=0" ] || pc_fail "-n 1 did not write the last line"
got=$(tail -n -1 f12 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "12${NL}rc=0" ] || pc_fail "-n -1 did not write the last line"
got=$(tail -n 0 f12 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "rc=0" ] || pc_fail "-n 0 did not write nothing"
got=$(tail -n 12 f12 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}2${NL}3${NL}4${NL}5${NL}6${NL}7${NL}8${NL}9${NL}10${NL}11${NL}12${NL}rc=0" ] || pc_fail "-n 12 on a twelve line file did not write it whole"
got=$(tail -n 50 f12 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}2${NL}3${NL}4${NL}5${NL}6${NL}7${NL}8${NL}9${NL}10${NL}11${NL}12${NL}rc=0" ] || pc_fail "-n 50 on a twelve line file did not write it whole"
got=$(tail -n 3 blank 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "${NL}y${NL}${NL}rc=0" ] || pc_fail "-n 3 did not count empty lines"
got=$(tail -n3 f12 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "10${NL}11${NL}12${NL}rc=0" ] || pc_fail "-n3 (option-argument in the same argument) did not write the last three lines"
# a file much larger than any single block of input
tail -n 3 big >out 2>/dev/null </dev/null || pc_fail "-n 3 on a large file failed"
cmp -s out bigtail || pc_fail "-n 3 on a large file did not write its last three lines"
tail -n +19999 big >out 2>/dev/null </dev/null || pc_fail "-n +19999 on a large file failed"
cmp -s out bigplus || pc_fail "-n +19999 on a large file did not write its last two lines"
tail -n +1 big >out 2>/dev/null </dev/null || pc_fail "-n +1 on a large file failed"
cmp -s out big || pc_fail "-n +1 on a large file did not copy it whole"
pc_pass
