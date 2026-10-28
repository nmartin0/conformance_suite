# When the action requested by an option or option-argument cannot be performed, the utility writes a diagnostic and exits non-zero: sort -o into a missing directory, and sed, grep and awk given an unreadable script or pattern file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.4-utility-defaults.019
. "$PC_CORE/pc.sh"
pc_begin XCU.1.4-utility-defaults.019
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'b\na\n' >f || pc_unresolved "cannot write a file"
pc_test
chk() {
	label=$1; shift
	"$@" >o 2>e </dev/null
	rc=$?
	[ "$rc" -ne 0 ] || pc_fail "$label: exit status 0"
	[ -s e ] || pc_fail "$label: no diagnostic"
}
chk "sort -o" sort -o nodir/out f
[ ! -e nodir/out ] || pc_fail "sort -o created a file in a missing directory"
chk "sed -f" sed -f nonexistent f
chk "grep -f" grep -f nonexistent f
chk "awk -f" awk -f nonexistent f
pc_pass
