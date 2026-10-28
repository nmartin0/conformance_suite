# Exit status of nohup.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nohup.003
. "$PC_CORE/pc.sh"
pc_begin XCU.nohup.003
nl='
'
shp=$(command -v "${PC_SH:-sh}")
case $shp in
/*) ;;
*) pc_unresolved "setup: cannot find an absolute path for sh ('$shp')" ;;
esac
printf 'exit 0\n' >noexec || pc_unresolved "setup: cannot create a file"
chmod 644 noexec || pc_unresolved "setup: chmod failed"
pc_test
nohup "$shp" -c 'exit 5' </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 5 ] || pc_fail "nohup sh -c 'exit 5' exited $rc, wanted 5"
nohup true </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 0 ] || pc_fail "nohup true exited $rc"
nohup false </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 1 ] || pc_fail "nohup false exited $rc, wanted 1"
nohup pc_no_such_utility_zz </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 127 ] || pc_fail "nohup of a missing utility exited $rc, wanted 127"
nohup ./noexec </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 126 ] || pc_fail "nohup of a file that cannot be run exited $rc, wanted 126"
pc_pass
