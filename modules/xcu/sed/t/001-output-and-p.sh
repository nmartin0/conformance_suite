# sed copies each line to the output after the script is applied, -n suppresses that copy, and p writes the pattern space.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sed.001
. "$PC_CORE/pc.sh"
pc_begin XCU.sed.001
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'one\ntwo\nthree\n' >in || pc_fail "cannot create input"
pc_test
got=$(sed 's/o/0/' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "0ne${NL}tw0${NL}three${NL}rc=0" ] || pc_fail "default output wrong"
got=$(sed -n '2p' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "two${NL}rc=0" ] || pc_fail "-n with 2p did not give only line 2"
got=$(sed '2p' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "one${NL}two${NL}two${NL}three${NL}rc=0" ] || pc_fail "p without -n did not duplicate line 2"
got=$(sed -n 's/e/E/' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "rc=0" ] || pc_fail "-n with no p wrote something"
got=$(sed 's/t/T/' <in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "one${NL}Two${NL}Three${NL}rc=0" ] || pc_fail "standard input was not edited"
pc_pass
