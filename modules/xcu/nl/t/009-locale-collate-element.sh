# A bracket expression of nl can name a collating element of the locale that LC_COLLATE (or LC_ALL, or LANG) names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nl.009
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.nl.009
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make
pcl_lonly=1
printf 'c\nch\nh\n' >n.txt || pc_unresolved "cannot write the input"
# only the line ch matches, so it is the only numbered line
pcl_probe() {
	nl -b p'^[[.ch.]]$' n.txt 2>/dev/null | sed -n '/^ *[[:digit:]]/s/^ *[[:digit:]]*[[:space:]]*//p'
}
pcl_want() {
	printf 'ch'
}
pc_test
pcloc_matrix "$PCLOC" LC_COLLATE
pc_pass
