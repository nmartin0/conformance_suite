# grep takes patterns from every -e and -f and the operand, and a null pattern from any of them matches every line.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.grep.003
. "$PC_CORE/pc.sh"
pc_begin XCU.grep.003
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'one\ntwo\nthree\nfour\n' >in || pc_fail "cannot create input"
printf 'one\nfour\n' >pats || pc_fail "cannot create pattern file"
printf 'two\n\n' >nullpats || pc_fail "cannot create pattern file"
pc_test
got=$(grep -e one -e three in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "one${NL}three${NL}rc=0" ] || pc_fail "two -e options were not both used"
got=$(grep -f pats in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "one${NL}four${NL}rc=0" ] || pc_fail "-f patterns were not both used"
got=$(grep -f pats -e two in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "one${NL}two${NL}four${NL}rc=0" ] || pc_fail "-f and -e were not combined"
got=$(grep -f nullpats in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "one${NL}two${NL}three${NL}four${NL}rc=0" ] || pc_fail "an empty line in a -f file did not match every line"
got=$(grep -e "three${NL}one" in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "one${NL}three${NL}rc=0" ] || pc_fail "a newline-separated -e list was not split"
got=$(grep -e -x in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "rc=1" ] || pc_fail "-e did not take a pattern starting with a hyphen-minus"
pc_pass
