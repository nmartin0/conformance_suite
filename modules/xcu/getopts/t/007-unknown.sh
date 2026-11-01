# An unknown option gives ? , unset OPTARG, a diagnostic and exit 0.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getopts.007
# MUTANT    getopts.unknown_status1
# MUTANT    getopts.unknown_status2
# MUTANT    getopts.unknown_name_opt
# MUTANT    getopts.unknown_optarg_set
# MUTANT    getopts.unknown_optarg_stale
# MUTANT    getopts.unknown_no_diag
# MUTANT    getopts.diag_stdout
# MUTANT    getopts.diag_both
# VARIANT   getopts.reimpl
# VARIANT   getopts.optind_opaque
# VARIANT   getopts.diag_fullpath
# VARIANT   getopts.diag_usage
. "$PC_CORE/pc.sh"
pc_begin XCU.getopts.007
pc_test
set -- -Q
OPTARG=stale
getopts ab pcopt 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exit $rc, wanted 0"
[ "$pcopt" = '?' ] || pc_fail "name is '$pcopt', wanted ?"
[ "${OPTARG+set}" != set ] || pc_fail "OPTARG is set to '$OPTARG'"
[ -s err ] || pc_fail "no diagnostic on standard error"
pc_pass
