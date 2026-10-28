# Under -P the data line has the file system name, total, used and free space, a whole percentage of used space rounded up, and the mount directory.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.df.003
. "$PC_CORE/pc.sh"
pc_begin XCU.df.003
pc_test
df -P . >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "df -P exited with status $rc"
n=$(sed -n '$=' out)
[ "$n" = 2 ] || pc_fail "expected 2 lines, found $n"
set -- $(sed -n 2p out)
[ "$#" -eq 6 ] || pc_unresolved "unexpected number of fields ($#) in the data line"
total=$2; used=$3; avail=$4; pct=$5; root=$6
case $total$used in
''|*[!0-9]*) pc_fail "total '$total' or used '$used' is not a number" ;;
esac
case $avail in
''|*[!0-9-]*) pc_fail "available '$avail' is not a number" ;;
esac
case $pct in
[0-9]*%) pct=${pct%\%} ;;
*) pc_fail "capacity '$pct' is not a percentage" ;;
esac
case $pct in
''|*[!0-9]*) pc_fail "capacity '$pct%' is not a whole percentage" ;;
esac
case $root in
/*) ;;
*) pc_fail "mount point '$root' is not an absolute pathname" ;;
esac
den=$((used + avail))
if [ "$den" -gt 0 ]; then
	want=$(((100 * used + den - 1) / den))
	[ "$pct" -eq "$want" ] || pc_fail "capacity is $pct%, but used $used and available $avail give $want%"
fi
pc_pass
