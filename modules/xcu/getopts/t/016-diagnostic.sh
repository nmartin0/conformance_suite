# The diagnostic names the program and the option.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getopts.016
. "$PC_CORE/pc.sh"
pc_begin XCU.getopts.016
base=${0##*/}
pc_test
set -- -Q
getopts ab pcopt 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exit $rc, wanted 0"
msg=$(cat err)
case $msg in
*"$base"*) ;;
*) pc_fail "diagnostic '$msg' does not name the program ($base)" ;;
esac
case $msg in
*Q*) ;;
*) pc_fail "diagnostic '$msg' does not name the option Q" ;;
esac
pc_pass
