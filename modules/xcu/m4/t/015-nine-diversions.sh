# diversions 1 to 9
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.015
# Diversions 1 to 9 all exist and are written in numeric order at the end of input.
. "$PC_CORE/pc.sh"
pc_begin XCU.m4.015
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
n=9
: >in
while [ "$n" -ge 1 ]; do
	echo "divert($n)t$n" >>in
	n=$((n-1))
done
printf 't1\nt2\nt3\nt4\nt5\nt6\nt7\nt8\nt9\n' >exp
pc_test
m4 in >out 2>err </dev/null || pc_fail "m4 failed"
cmp -s out exp || pc_fail "output was '$(tr '\n' ' ' <out)', expected '$(tr '\n' ' ' <exp)'"
pc_pass
