# sed treats a multibyte character of the code set that LC_CTYPE (or LC_ALL, or LANG) names as one character.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sed.024
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.sed.024
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make_mb
pcl_basec="LC_COLLATE LC_NUMERIC LC_TIME LC_MONETARY LC_MESSAGES"
pcl_lonly=1
pcl_probe() {
	printf 'a\341\342b\n' | sed 's/./X/g' 2>/dev/null
}
pcl_want() {
	printf 'XXX'
}
pc_test
pcloc_matrix "$PCLOCMB" LC_CTYPE
pc_pass
