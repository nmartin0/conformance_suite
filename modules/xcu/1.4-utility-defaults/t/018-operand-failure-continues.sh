# When the requested action cannot be performed on one operand, the utility writes a diagnostic, goes on to the next operand and exits with a non-zero status: cat, rm, chmod, wc, head, cksum, mv, rmdir, ls, cp, mkdir and touch
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.4-utility-defaults.018
. "$PC_CORE/pc.sh"
pc_begin XCU.1.4-utility-defaults.018
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'x\n' >f || pc_unresolved "cannot write a file"
chmod 644 f || pc_unresolved "chmod"
pc_test
nl='
'
chk() {
	# chk label cmd...: must fail, diagnose, and have done the work for the good operand
	label=$1; shift
	"$@" >o 2>e </dev/null
	rc=$?
	[ "$rc" -ne 0 ] || pc_fail "$label: exit status 0 although an operand failed"
	[ -s e ] || pc_fail "$label: no diagnostic"
}
chk cat cat nonexist f
[ "$(cat o)" = x ] || pc_fail "cat: the good operand was not processed"
printf 'y\n' >f2 || pc_unresolved "write"
chk rm rm nonexist f2
[ ! -e f2 ] || pc_fail "rm: the good operand was not removed"
chk chmod chmod 600 nonexist f
m=$(ls -l f | cut -c 1-10)
[ "$m" = "-rw-------" ] || pc_fail "chmod: the good operand has mode $m"
chk wc wc -c nonexist f
grep ' f$' o >/dev/null || pc_fail "wc: no count for the good operand"
chk head head -n 1 nonexist f
grep x o >/dev/null || pc_fail "head: the good operand was not processed"
chk cksum cksum nonexist f
grep ' f$' o >/dev/null || pc_fail "cksum: no checksum for the good operand"
mkdir d || pc_unresolved "mkdir"
printf 'z\n' >g || pc_unresolved "write"
chk mv mv nonexist g d/
[ -f d/g ] || pc_fail "mv: the good operand was not moved"
mkdir d2 || pc_unresolved "mkdir"
chk rmdir rmdir nonexist d2
[ ! -d d2 ] || pc_fail "rmdir: the good directory was not removed"
chk ls ls nonexist d
grep g o >/dev/null || pc_fail "ls: the good operand was not listed"
printf 'q\n' >h || pc_unresolved "write"
chk cp cp nonexist h d/
[ -f d/h ] || pc_fail "cp: the good operand was not copied"
chk mkdir mkdir nodir/x d3
[ -d d3 ] || pc_fail "mkdir: the good operand was not created"
chk touch touch nodir/x t3
[ -f t3 ] || pc_fail "touch: the good operand was not created"
pc_pass
