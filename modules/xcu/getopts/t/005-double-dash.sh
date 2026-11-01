# -- ends the options.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getopts.005
# MUTANT    getopts.ddash_status0
# MUTANT    getopts.ddash_continue
# MUTANT    getopts.ddash_optind_at
# MUTANT    getopts.ddash_optind_plus1
# MUTANT    getopts.ddash_name_kept
# MUTANT    getopts.ddash_arg_missing
# VARIANT   getopts.reimpl
# VARIANT   getopts.optind_opaque
. "$PC_CORE/pc.sh"
pc_begin XCU.getopts.005
pc_test
set -- -a -- -b
getopts ab pcopt
rc1=$?
getopts ab pcopt
rc2=$?
v2=$pcopt
ind=$OPTIND
[ "$rc1" -eq 0 ] || pc_fail "first call exited $rc1"
[ "$rc2" -eq 1 ] || pc_fail "call at -- exited $rc2, wanted 1"
[ "$v2" = '?' ] || pc_fail "name is '$v2' at --, wanted ?"
[ "$ind" = 3 ] || pc_fail "OPTIND is '$ind' after --, wanted 3"
pc_pass
