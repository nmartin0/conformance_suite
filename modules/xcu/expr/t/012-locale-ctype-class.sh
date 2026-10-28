# A character class in an expr match follows the locale that LC_CTYPE (or LC_ALL, or LANG) names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.expr.012
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.expr.012
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make
pcl_basec="LC_COLLATE LC_NUMERIC LC_TIME LC_MONETARY LC_MESSAGES"
pcl_lonly=1
pcl_probe() {
	pcl_ch=$(printf '\341')
	expr "$pcl_ch" : '[[:lower:]]$' 2>/dev/null
}
pcl_want() {
	printf 1
}
pc_test
pcloc_matrix "$PCLOC" LC_CTYPE
pc_pass
