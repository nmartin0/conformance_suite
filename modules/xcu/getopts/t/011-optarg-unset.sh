# OPTARG is unset after a flag and at the end of options.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getopts.011
# MUTANT    getopts.flag_optarg_stale
# MUTANT    getopts.flag_optarg_empty
# MUTANT    getopts.end_optarg_stale
# MUTANT    getopts.end_optarg_empty
# VARIANT   getopts.reimpl
# VARIANT   getopts.optind_opaque
. "$PC_CORE/pc.sh"
pc_begin XCU.getopts.011
pc_test
set -- -a
OPTARG=stale
getopts a pcopt
rc=$?
[ "$rc" -eq 0 ] && [ "$pcopt" = a ] || pc_fail "flag not found: exit $rc, name '$pcopt'"
[ "${OPTARG+set}" != set ] || pc_fail "OPTARG is set to '$OPTARG' after a flag"
OPTARG=stale2
getopts a pcopt
rc=$?
[ "$rc" -eq 1 ] || pc_fail "end of options exited $rc"
[ "${OPTARG+set}" != set ] || pc_fail "OPTARG is set to '$OPTARG' at the end of options"
pc_pass
