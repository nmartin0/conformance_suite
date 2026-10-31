# With no file operand tail reads standard input.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tail.005
# MUTANT    tail.stdin_ignored
# MUTANT    tail.stdin_opts_ignored
# MUTANT    tail.stdin_pipe_fails
# MUTANT    tail.stdin_plus_ignored
# MUTANT    tail.operand_ignored
# MUTANT    tail.stdin_with_operand
# VARIANT   tail.byte_writes
. "$PC_CORE/pc.sh"
pc_begin XCU.tail.005
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '%s\n' 1 2 3 4 5 6 7 8 9 10 11 12 >f12 || pc_fail "cannot create input"
printf 'p\nq\nr\n' >other || pc_fail "cannot create input"
pc_test
got=$(tail -n 2 <f12 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "11${NL}12${NL}rc=0" ] || pc_fail "-n 2 on standard input is wrong"
got=$(tail <f12 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "3${NL}4${NL}5${NL}6${NL}7${NL}8${NL}9${NL}10${NL}11${NL}12${NL}rc=0" ] || pc_fail "default count on standard input is wrong"
got=$(cat f12 | tail -n 2 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "11${NL}12${NL}rc=0" ] || pc_fail "-n 2 on standard input from a pipe is wrong"
got=$(cat f12 | tail 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "3${NL}4${NL}5${NL}6${NL}7${NL}8${NL}9${NL}10${NL}11${NL}12${NL}rc=0" ] || pc_fail "default count on standard input from a pipe is wrong"
got=$(tail -n +11 <f12 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "11${NL}12${NL}rc=0" ] || pc_fail "-n +11 on standard input is wrong"
got=$(cat f12 | tail -n +11 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "11${NL}12${NL}rc=0" ] || pc_fail "-n +11 on standard input from a pipe is wrong"
got=$(tail -c 6 <f12 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "11${NL}12${NL}rc=0" ] || pc_fail "-c 6 on standard input is wrong"
# with a file operand, standard input is not used
got=$(tail -n 1 f12 <other 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "12${NL}rc=0" ] || pc_fail "a file operand did not take the place of standard input"
pc_pass
