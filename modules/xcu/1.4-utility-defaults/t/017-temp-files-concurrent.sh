# Several instances of the same utility running at once in one working directory and sharing one TMPDIR do not disturb each other: six concurrent sorts each produce the correct output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.4-utility-defaults.017
. "$PC_CORE/pc.sh"
pc_begin XCU.1.4-utility-defaults.017
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mkdir tmp || pc_unresolved "cannot make the temporary directory"
for i in 1 2 3 4 5 6; do
	awk -v s=$i 'BEGIN { for (n = 0; n < 50000; n++) print (n * 7919 * s) % 100003 }' >in$i || pc_unresolved "cannot make an input"
	sort in$i >exp$i || pc_unresolved "cannot make the expected output"
done
pc_test
TMPDIR=$PWD/tmp; export TMPDIR
for i in 1 2 3 4 5 6; do
	sort in$i >out$i 2>err$i &
done
wait
for i in 1 2 3 4 5 6; do
	cmp -s out$i exp$i || pc_fail "concurrent sort number $i produced wrong output"
done
left=$(ls -A tmp)
[ -z "$left" ] || pc_fail "temporary files left behind: $left"
pc_pass
