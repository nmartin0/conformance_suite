# The diagnostic names the program and the option.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getopts.016
# MUTANT    getopts.diag_no_prog
# MUTANT    getopts.diag_prog_getopts
# MUTANT    getopts.diag_unknown_no_opt
# MUTANT    getopts.diag_missing_no_prog
# MUTANT    getopts.diag_missing_no_opt
# MUTANT    getopts.diag_stdout
# VARIANT   getopts.reimpl
# VARIANT   getopts.diag_fullpath
# VARIANT   getopts.diag_usage
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
# a missing option-argument: the program and the option are named too
OPTIND=1
set -- -Z
getopts c:Z: pcopt 2>err2
rc=$?
[ "$rc" -eq 0 ] || pc_fail "missing argument: exit $rc, wanted 0"
msg=$(cat err2)
case $msg in
*"$base"*) ;;
*) pc_fail "diagnostic '$msg' for a missing argument does not name the program ($base)" ;;
esac
case $msg in
*Z*) ;;
*) pc_fail "diagnostic '$msg' for a missing argument does not name the option Z" ;;
esac
pc_pass
