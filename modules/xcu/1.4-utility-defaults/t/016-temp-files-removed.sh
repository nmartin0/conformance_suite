# Temporary files that standard utilities create in TMPDIR are removed on both successful exit and exit because of errors: after sort, ed, sed, awk and diff finish, nothing is left in the directory
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.4-utility-defaults.016
. "$PC_CORE/pc.sh"
pc_begin XCU.1.4-utility-defaults.016
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mkdir tmp || pc_unresolved "cannot make the temporary directory"
awk 'BEGIN { for (i = 0; i < 100000; i++) print (i * 7919) % 100003 }' >big || pc_unresolved "cannot make the input"
cp big big2 || pc_unresolved "cannot copy the input"
printf 'one\ntwo\n' >f || pc_unresolved "cannot write a file"
pc_test
TMPDIR=$PWD/tmp; export TMPDIR
empty() {
	left=$(ls -A tmp)
	[ -z "$left" ] || pc_fail "$1 left temporary files: $left"
}
sort big >o 2>e; empty "sort"
sort nonexistent >o 2>e; empty "sort (error exit)"
sed -n p big >o 2>e; empty "sed"
sed -f nonexistent big >o 2>e; empty "sed (error exit)"
awk '{ n++ } END { print n }' big >o 2>e; empty "awk"
diff big big2 >o 2>e; empty "diff"
diff big nonexistent >o 2>e; empty "diff (error exit)"
printf 'w\nq\n' | ed -s f >o 2>e; empty "ed"
printf 'x\nq\n' | ed -s nonexistent >o 2>e; empty "ed (error exit)"
pc_pass
