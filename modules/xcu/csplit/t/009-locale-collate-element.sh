# A bracket expression of csplit can name a collating element of the locale that LC_COLLATE (or LC_ALL, or LANG) names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.csplit.009
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.csplit.009
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make
pcl_lonly=1
printf 'x\nch\nc\n' >cs.in || pc_unresolved "cannot write the input"
# the first line that matches is the second, so the first piece holds one line
pcl_probe() {
	rm -f cs.out00 cs.out01
	csplit -s -f cs.out cs.in '/^[[.ch.]]$/' >/dev/null 2>&1
	cat cs.out00 2>/dev/null
}
pcl_want() {
	printf 'x'
}
pc_test
pcloc_matrix "$PCLOC" LC_COLLATE
pc_pass
