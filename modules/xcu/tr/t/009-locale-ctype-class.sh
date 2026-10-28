# tr classifies characters and maps case as the locale that LC_CTYPE (or LC_ALL, or LANG) names says.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tr.009
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.tr.009
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make
pcl_basec="LC_COLLATE LC_NUMERIC LC_TIME LC_MONETARY LC_MESSAGES"
pcl_lonly=1
# in the locale 0xe1 is a lowercase letter whose uppercase equivalent is 0xc1
pcl_probe() {
	pcl_a=$(printf 'a\341B\n' | tr -d '[:lower:]' 2>/dev/null | pcloc_hex)
	pcl_b=$(printf '\341\n' | tr '[:lower:]' '[:upper:]' 2>/dev/null | pcloc_hex)
	printf '%s|%s' "$pcl_a" "$pcl_b"
}
pcl_want() {
	printf '420a|c10a'
}
pc_test
pcloc_matrix "$PCLOC" LC_CTYPE
pc_pass
