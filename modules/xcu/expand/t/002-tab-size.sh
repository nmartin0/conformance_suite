# expand -t number sets tab stops that many columns apart.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.expand.002
. "$PC_CORE/pc.sh"
pc_begin XCU.expand.002
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
printf 'a%sb\n' "$T" >in1 && printf '%s%sx\n' "$T" "$T" >in2 && printf 'abcd%sx\n' "$T" >in3 || pc_fail "cannot create input"
pc_test
got=$(expand -t 4 in1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a   b${NL}rc=0" ] || pc_fail "-t 4 did not give three spaces after one character"
got=$(expand -t 4 in2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "        x${NL}rc=0" ] || pc_fail "-t 4 did not give eight spaces for two tabs"
got=$(expand -t 4 in3 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "abcd    x${NL}rc=0" ] || pc_fail "-t 4 did not go to the next stop from a stop"
got=$(expand -t 2 in1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a b${NL}rc=0" ] || pc_fail "-t 2 did not give one space after one character"
pc_pass
