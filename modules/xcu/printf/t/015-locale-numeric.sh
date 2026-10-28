# The e, f and g conversions of printf write the radix character of the locale that LC_NUMERIC (or LC_ALL, or LANG) names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.printf.015
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.printf.015
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make
pcl_v=$(env printf '%.1f' 1 2>/dev/null)
[ "$pcl_v" = 1.0 ] || pc_notinuse "the printf utility does not support the f conversion (optional)"
pcl_probe() {
	env printf '%.1f' 1 2>/dev/null
}
pcl_want() {
	case $1 in
	L) printf '1,0' ;;
	C) printf '1.0' ;;
	esac
}
pc_test
pcloc_matrix "$PCLOC" LC_NUMERIC
pc_pass
