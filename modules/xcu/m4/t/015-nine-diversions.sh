# diversions 1 to 9
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.015
# Diversions 1 to 9 all exist and are written in numeric order at the end of input.
# MUTANT    m4.divert_max8
# MUTANT    m4.divert_9_is_1
# MUTANT    m4.divs_first
# MUTANT    m4.divs_reverse
# MUTANT    m4.no_flush_divs
# MUTANT    m4.divert_ignored
. "$PC_CORE/pc.sh"
pc_begin XCU.m4.015
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
echo head >in
n=9
while [ "$n" -ge 1 ]; do
	echo "divert($n)t$n" >>in
	n=$((n-1))
done
echo "divert(0)tail" >>in
printf 'head\ntail\nt1\nt2\nt3\nt4\nt5\nt6\nt7\nt8\nt9\n' >exp
pc_test
m4 in >out 2>err </dev/null || pc_fail "m4 failed"
cmp -s out exp || pc_fail "output was '$(tr '\n' ' ' <out)', expected '$(tr '\n' ' ' <exp)'"
pc_pass
