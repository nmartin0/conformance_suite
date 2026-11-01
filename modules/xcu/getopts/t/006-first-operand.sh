# A non-option word ends the options.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getopts.006
# MUTANT    getopts.operand_continue
# MUTANT    getopts.operand_status0
# MUTANT    getopts.operand_optind_after
# MUTANT    getopts.operand_name_kept
# MUTANT    getopts.end_noperand_clamp
# VARIANT   getopts.reimpl
# VARIANT   getopts.optind_opaque
. "$PC_CORE/pc.sh"
pc_begin XCU.getopts.006
pc_test
set -- -a file -b
getopts ab pcopt
rc1=$?
getopts ab pcopt
rc2=$?
ind=$OPTIND
[ "$rc1" -eq 0 ] || pc_fail "first call exited $rc1"
[ "$rc2" -eq 1 ] || pc_fail "call at the operand exited $rc2, wanted 1"
[ "$ind" = 2 ] || pc_fail "OPTIND is '$ind', wanted 2"
pc_pass
