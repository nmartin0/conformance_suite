# option syntax
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sed.016
# Options can be grouped (-ne, -nE), -e takes its script attached or separate,
# and -- ends the options before a script operand.
. "$PC_CORE/pc.sh"
pc_begin XCU.sed.016
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'one\ntwo\n' >in || pc_fail "cannot create input"
pc_test
got=$(sed -ne 2p in 2>/dev/null </dev/null)
[ "$got" = "two" ] || pc_fail "-ne 2p: $got"
got=$(sed -n -e 2p in 2>/dev/null </dev/null)
[ "$got" = "two" ] || pc_fail "-n -e 2p: $got"
got=$(sed -nE '/(o)n/p' in 2>/dev/null </dev/null)
[ "$got" = "one" ] || pc_fail "-nE: $got"
got=$(sed -n -- 1p in 2>/dev/null </dev/null)
[ "$got" = "one" ] || pc_fail "-- before the script: $got"
got=$(sed -n -e 1p -e 2p in 2>/dev/null </dev/null)
[ "$got" = "one${NL}two" ] || pc_fail "two -e options: $got"
pc_pass
