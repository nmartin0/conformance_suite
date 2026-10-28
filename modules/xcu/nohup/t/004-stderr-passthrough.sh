# nohup leaves a non-terminal standard error alone.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nohup.004
. "$PC_CORE/pc.sh"
pc_begin XCU.nohup.004
nl='
'
shp=$(command -v "${PC_SH:-sh}")
case $shp in
/*) ;;
*) pc_unresolved "setup: cannot find an absolute path for sh ('$shp')" ;;
esac
pc_test
nohup "$shp" -c 'echo oops >&2; echo fine' </dev/null >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "nohup exited $rc"
got=$(cat err)
[ "$got" = oops ] || pc_fail "standard error held '$got', wanted just oops"
got=$(cat out)
[ "$got" = fine ] || pc_fail "standard output held '$got', wanted just fine"
pc_pass
