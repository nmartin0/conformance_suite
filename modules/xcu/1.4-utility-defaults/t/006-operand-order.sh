# Standard utilities process their operands in the order given on the command line: cat, wc, head, tail, cksum and grep report files in operand order
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.4-utility-defaults.006
. "$PC_CORE/pc.sh"
pc_begin XCU.1.4-utility-defaults.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
nl='
'
printf 'A\n' >fa; printf 'B\n' >fb; printf 'C\n' >fc || pc_unresolved "cannot write the files"
pc_test
got=$(cat fc fa fb)
[ "$got" = "C${nl}A${nl}B" ] || pc_fail "cat: got '$got'"
got=$(wc -l fb fc fa | while read n name; do printf '%s ' "$name"; done)
[ "$got" = "fb fc fa total " ] || pc_fail "wc: file order '$got'"
got=$(head -n 1 fc fa | sed -n '/^==>/p')
[ "$got" = "==> fc <==${nl}==> fa <==" ] || pc_fail "head: headers '$got'"
got=$(tail -n 1 fb fa | sed -n '/^==>/p')
[ "$got" = "==> fb <==${nl}==> fa <==" ] || pc_fail "tail: headers '$got'"
got=$(cksum fb fa fc | while read crc size name; do printf '%s ' "$name"; done)
[ "$got" = "fb fa fc " ] || pc_fail "cksum: file order '$got'"
got=$(grep . fc fb)
[ "$got" = "fc:C${nl}fb:B" ] || pc_fail "grep: got '$got'"
pc_pass
