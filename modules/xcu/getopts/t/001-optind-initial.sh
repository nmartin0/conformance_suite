# OPTIND is 1 when the shell starts.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getopts.001
# MUTANT    getopts.optind_zero
# MUTANT    getopts.optind_two
# VARIANT   getopts.reimpl
. "$PC_CORE/pc.sh"
pc_begin XCU.getopts.001
pc_test
[ "${OPTIND-unset}" = 1 ] || pc_fail "OPTIND is '${OPTIND-unset}' at shell start"
pc_pass
