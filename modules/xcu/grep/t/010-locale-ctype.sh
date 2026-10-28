# grep classifies characters and ignores case as the locale that LC_CTYPE (or LC_ALL, or LANG) names says.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.grep.010
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.grep.010
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make
pcl_basec="LC_COLLATE LC_NUMERIC LC_TIME LC_MONETARY LC_MESSAGES"
pcl_lonly=1
printf '\341\n\301\n1\n' >g2.txt || pc_unresolved "cannot write the input"
# in the locale 0xe1 is a lowercase letter whose uppercase equivalent is 0xc1
pcl_probe() {
	pcl_ch=$(printf '\341')
	pcl_a=$(grep -c '[[:lower:]]' g2.txt 2>/dev/null)
	pcl_b=$(grep -i -c "$pcl_ch" g2.txt 2>/dev/null)
	printf '%s|%s' "$pcl_a" "$pcl_b"
}
pcl_want() {
	printf '1|2'
}
pc_test
pcloc_matrix "$PCLOC" LC_CTYPE
pc_pass
