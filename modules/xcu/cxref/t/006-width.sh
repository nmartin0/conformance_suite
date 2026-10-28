# -w limits the line width
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cxref.006
# With -w and a number of 51 or more, no output line is wider than that many
# columns even for a symbol with many references.
. "$PC_CORE/pc.sh"
pc_begin XCU.cxref.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
i=0
{
	echo 'int counter;'
	echo 'int'
	echo 'run(void)'
	echo '{'
	while [ $i -lt 40 ]; do
		echo '	counter = counter + 1;'
		i=$((i + 1))
	done
	echo '	return counter;'
	echo '}'
} >x.c
wide() { awk -v w="$2" 'length($0) > w { bad = 1 } END { exit bad }' "$1"; }
pc_test
for w in 51 60 100; do
	cxref -w $w x.c </dev/null >o.$w 2>err || pc_fail "cxref -w $w: nonzero status"
	[ -s o.$w ] || pc_fail "cxref -w $w wrote nothing"
	wide o.$w $w || pc_fail "-w $w: a line is wider than $w columns"
	grep -w 40 o.$w >/dev/null || pc_fail "-w $w: references are missing"
done
pc_pass
