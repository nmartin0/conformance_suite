# A line beginning with 1 is preceded by characters that advance to the next page.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.asa.003
. "$PC_CORE/pc.sh"
pc_begin XCU.asa.003
LC_ALL=POSIX
export LC_ALL
NL='
'
printf ' a\n1b\n' >in || pc_fail "cannot create input"
pc_test
asa <in >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "status was not 0"
got=$(cat out; printf 'x')
case $got in
"a${NL}"*"b${NL}x") ;;
*) pc_fail "the output was not a, a line end, something, and b" ;;
esac
mid=${got#a$NL}
mid=${mid%"b${NL}x"}
[ -n "$mid" ] || pc_fail "nothing was written before b to advance to the next page"
printf ' a\n b\n' | asa >out2 2>/dev/null
got2=$(cat out2; printf 'x')
[ "$got2" != "$got" ] || pc_fail "1 and space gave the same output"
pc_pass
