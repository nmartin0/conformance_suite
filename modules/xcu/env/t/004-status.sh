# Exit status of env.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.env.004
. "$PC_CORE/pc.sh"
pc_begin XCU.env.004
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
env "$shp" -c 'exit 7' </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 7 ] || pc_fail "env sh -c 'exit 7' exited $rc, wanted 7"
env true </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 0 ] || pc_fail "env true exited $rc"
env false </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 1 ] || pc_fail "env false exited $rc, wanted 1"
env pc_no_such_utility_zz </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 127 ] || pc_fail "env of a missing utility exited $rc, wanted 127"
env ./noexec </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 126 ] || pc_fail "env of a file that cannot be run exited $rc, wanted 126"
(exec env "$shp" -c 'exit 3') </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 3 ] || pc_fail "exec env sh -c 'exit 3' exited $rc, wanted 3"
pc_pass
