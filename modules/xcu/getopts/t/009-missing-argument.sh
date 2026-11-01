# A missing option-argument gives ? , unset OPTARG, a diagnostic and exit 0.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getopts.009
# MUTANT    getopts.missing_status1
# MUTANT    getopts.missing_status2
# MUTANT    getopts.missing_name_colon
# MUTANT    getopts.missing_name_opt
# MUTANT    getopts.missing_optarg_set
# MUTANT    getopts.missing_optarg_stale
# MUTANT    getopts.missing_no_diag
# MUTANT    getopts.diag_stdout
# MUTANT    getopts.diag_both
# VARIANT   getopts.reimpl
# VARIANT   getopts.optind_opaque
# VARIANT   getopts.diag_fullpath
# VARIANT   getopts.diag_usage
. "$PC_CORE/pc.sh"
pc_begin XCU.getopts.009
pc_test
set -- -c
OPTARG=stale
getopts c: pcopt 2>err >out
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exit $rc, wanted 0"
[ "$pcopt" = '?' ] || pc_fail "name is '$pcopt', wanted ?"
[ "${OPTARG+set}" != set ] || pc_fail "OPTARG is set to '$OPTARG'"
[ ! -s out ] || pc_fail "something was written to standard output"
[ -s err ] || pc_fail "no diagnostic on standard error"
pc_pass
