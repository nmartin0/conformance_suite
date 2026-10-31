# With no file operand head reads standard input.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.head.005
# MUTANT    head.stdin_ignored
# MUTANT    head.stdin_header
# MUTANT    head.stdin_consumed
# MUTANT    head.default_all
. "$PC_CORE/pc.sh"
pc_begin XCU.head.005
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '%s\n' 1 2 3 4 5 6 7 8 9 10 11 12 >f12 || pc_fail "cannot create input"
printf 'x\ny\n' >g || pc_fail "cannot create input"
pc_test
got=$(head -n 2 <f12 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}2${NL}rc=0" ] || pc_fail "-n 2 on standard input is wrong"
got=$(head <f12 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}2${NL}3${NL}4${NL}5${NL}6${NL}7${NL}8${NL}9${NL}10${NL}rc=0" ] || pc_fail "default count on standard input is wrong"
got=$( (head -n 1 g; cat) <f12 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "x${NL}1${NL}2${NL}3${NL}4${NL}5${NL}6${NL}7${NL}8${NL}9${NL}10${NL}11${NL}12${NL}rc=0" ] || pc_fail "standard input was used although a file operand was given"
pc_pass
