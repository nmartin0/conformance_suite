# Elements of a cut list may repeat, overlap and come in any order; the output follows the input order and shows each position once.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cut.003
. "$PC_CORE/pc.sh"
pc_begin XCU.cut.003
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abcdefgh\n' >in || pc_fail "cannot create input"
pc_test
got=$(cut -b 5,1-2,2-3,5 in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "abce${NL}rc=0" ] || pc_fail "-b 5,1-2,2-3,5 did not give abce"
got=$(cut -c 7-8,3,2-4 in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "bcdgh${NL}rc=0" ] || pc_fail "-c 7-8,3,2-4 did not give bcdgh"
printf 'a:b:c:d:e\n' >inf
got=$(cut -d : -f 4,2,2 inf 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "b:d${NL}rc=0" ] || pc_fail "-f 4,2,2 did not give b:d"
pc_pass
