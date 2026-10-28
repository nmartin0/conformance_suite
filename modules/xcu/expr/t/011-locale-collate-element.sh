# A bracket expression of an expr match can name a collating element of the locale that LC_COLLATE (or LC_ALL, or LANG) names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.expr.011
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.expr.011
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make
pcl_lonly=1
# ch is a collating element of the locale: it matches the two characters together and not c alone
pcl_probe() {
	pcl_a=$(expr ch : '[[.ch.]]$' 2>/dev/null)
	pcl_b=$(expr c : '[[.ch.]]$' 2>/dev/null)
	printf '%s %s' "$pcl_a" "$pcl_b"
}
pcl_want() {
	printf '2 0'
}
pc_test
pcloc_matrix "$PCLOC" LC_COLLATE
pc_pass
