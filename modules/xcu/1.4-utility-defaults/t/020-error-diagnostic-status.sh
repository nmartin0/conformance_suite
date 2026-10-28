# When a utility meets an error (a missing operand file or an impossible target), it writes a diagnostic to standard error and exits with a non-zero status
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.4-utility-defaults.020
. "$PC_CORE/pc.sh"
pc_begin XCU.1.4-utility-defaults.020
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'x\n' >f || pc_unresolved "cannot write a file"
pc_test
chk() {
	"$@" >o 2>e </dev/null
	rc=$?
	[ "$rc" -ne 0 ] || pc_fail "$*: exit status 0"
	[ -s e ] || pc_fail "$*: no diagnostic"
}
chk cat nonexist
chk head nonexist
chk tail nonexist
chk sort nonexist
chk wc nonexist
chk cksum nonexist
chk cmp nonexist f
chk diff nonexist f
chk rm nonexist
chk rmdir nonexist
chk mv nonexist g
chk cp nonexist g
chk ln nonexist g
chk ls nonexist
chk od nonexist
chk uniq nonexist
chk cut -c 1 nonexist
chk sed p nonexist
chk grep x nonexist
chk fold nonexist
chk paste nonexist
chk mkdir nodir/x
chk touch nodir/x
chk chmod 600 nonexist
chk du nonexist
chk find nonexist
chk expand nonexist
chk comm nonexist f
pc_pass
