# A character class in a csplit regular expression follows the locale that LC_CTYPE (or LC_ALL, or LANG) names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.csplit.010
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.csplit.010
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make
pcl_basec="LC_COLLATE LC_NUMERIC LC_TIME LC_MONETARY LC_MESSAGES"
pcl_lonly=1
printf '1\n\341\n' >cs2.in || pc_unresolved "cannot write the input"
# byte 0xe1 is a lowercase letter in the locale, so the first piece holds the line before it
pcl_probe() {
	rm -f cs2.out00 cs2.out01
	csplit -s -f cs2.out cs2.in '/^[[:lower:]]$/' >/dev/null 2>&1
	cat cs2.out00 2>/dev/null | pcloc_hex
}
pcl_want() {
	printf '310a'
}
pc_test
pcloc_matrix "$PCLOC" LC_CTYPE
pc_pass
