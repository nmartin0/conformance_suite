# With no options tail writes the last ten lines of its input, or all of it when shorter.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tail.001
# MUTANT    tail.default11
# MUTANT    tail.default9
# MUTANT    tail.default_all
# MUTANT    tail.default_bytes
# MUTANT    tail.short_nothing
# MUTANT    tail.empty_fails
# MUTANT    tail.ok_noise_stderr
# MUTANT    tail.status1
# MUTANT    tail.out_no_final_newline
# MUTANT    tail.out_extra_newline
# MUTANT    tail.out_to_stderr
# VARIANT   tail.byte_writes
# VARIANT   tail.buffer_limit
. "$PC_CORE/pc.sh"
pc_begin XCU.tail.001
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '%s\n' 1 2 3 4 5 6 7 8 9 10 11 12 >f12 || pc_fail "cannot create input"
printf '%s\n' 1 2 3 4 5 6 7 8 9 10 11 >f11 || pc_fail "cannot create input"
printf '%s\n' 1 2 3 4 5 6 7 8 9 10 >f10 || pc_fail "cannot create input"
printf '%s\n' 1 2 3 4 5 6 7 8 9 >f9 || pc_fail "cannot create input"
printf '%s\n' a b c >f3 || pc_fail "cannot create input"
: >fempty || pc_fail "cannot create input"
pc_test
got=$(tail f12 2>err </dev/null; printf 'rc=%s' "$?")
[ "$got" = "3${NL}4${NL}5${NL}6${NL}7${NL}8${NL}9${NL}10${NL}11${NL}12${NL}rc=0" ] || pc_fail "tail did not write exactly the last ten lines"
[ ! -s err ] || pc_fail "tail wrote to standard error although it succeeded"
got=$(tail f11 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "2${NL}3${NL}4${NL}5${NL}6${NL}7${NL}8${NL}9${NL}10${NL}11${NL}rc=0" ] || pc_fail "tail did not write the last ten of eleven lines"
got=$(tail f10 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}2${NL}3${NL}4${NL}5${NL}6${NL}7${NL}8${NL}9${NL}10${NL}rc=0" ] || pc_fail "tail did not copy a ten line file whole"
got=$(tail f9 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}2${NL}3${NL}4${NL}5${NL}6${NL}7${NL}8${NL}9${NL}rc=0" ] || pc_fail "tail did not copy a nine line file whole"
got=$(tail f3 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}b${NL}c${NL}rc=0" ] || pc_fail "tail did not copy a short file whole"
got=$(tail fempty 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "rc=0" ] || pc_fail "tail of an empty file did not write nothing and succeed"
pc_pass
