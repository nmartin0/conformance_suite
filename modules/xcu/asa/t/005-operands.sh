# asa reads the file operands in order, or the standard input when there is none, and exits 0 on success.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.asa.005
. "$PC_CORE/pc.sh"
pc_begin XCU.asa.005
LC_ALL=POSIX
export LC_ALL
NL='
'
printf ' one\n' >f1 && printf ' two\n0three\n' >f2 && : >empty || pc_fail "cannot create input"
pc_test
got=$(asa f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "one${NL}two${NL}${NL}three${NL}rc=0" ] || pc_fail "two files were not written in order"
got=$(cat f2 | asa 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "two${NL}${NL}three${NL}rc=0" ] || pc_fail "standard input was not read"
got=$(asa empty 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "rc=0" ] || pc_fail "an empty file gave output or a nonzero status"
pc_pass
