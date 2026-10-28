# A bracket expression of grep can name a collating element of the locale that LC_COLLATE (or LC_ALL, or LANG) names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.grep.009
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.grep.009
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make
pcl_lonly=1
printf 'ch\nc\nh\nxch\n' >g.txt || pc_unresolved "cannot write the input"
# ch is a collating element of the locale: it matches the two characters together and not c or h alone
pcl_probe() {
	grep '^[[.ch.]]$' g.txt 2>/dev/null
}
pcl_want() {
	printf 'ch'
}
pc_test
pcloc_matrix "$PCLOC" LC_COLLATE
pc_pass
