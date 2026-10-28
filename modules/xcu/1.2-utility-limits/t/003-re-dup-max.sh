# An interval expression may repeat 255 times in a BRE and an ERE: a{255} matches a line of exactly 255 a characters and does not match 254
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.2-utility-limits.003
. "$PC_CORE/pc.sh"
pc_begin XCU.1.2-utility-limits.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
awk 'BEGIN { s = ""; for (i = 0; i < 255; i++) s = s "a"; print s; print substr(s, 2) }' >in || pc_unresolved "cannot build the input"
pc_test
n=$(grep -c '^a\{255\}$' in) || pc_fail "grep BRE failed"
[ "$n" = 1 ] || pc_fail "grep BRE matched $n lines, not 1"
n=$(grep -Ec '^a{255}$' in) || pc_fail "grep ERE failed"
[ "$n" = 1 ] || pc_fail "grep ERE matched $n lines, not 1"
n=$(sed -n '/^a\{255\}$/p' in | wc -l | tr -d ' ')
[ "$n" = 1 ] || pc_fail "sed BRE matched $n lines, not 1"
n=$(sed -n '/^a\{254\}$/p' in | wc -l | tr -d ' ')
[ "$n" = 1 ] || pc_fail "sed BRE with 254 matched $n lines, not 1"
pc_pass
