# With a leading colon a missing option-argument gives : and OPTARG.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getopts.010
# MUTANT    getopts.mcolon_name_q
# MUTANT    getopts.mcolon_name_opt
# MUTANT    getopts.mcolon_optarg_unset
# MUTANT    getopts.mcolon_optarg_colon
# MUTANT    getopts.mcolon_diag
# MUTANT    getopts.mcolon_status1
# VARIANT   getopts.reimpl
# VARIANT   getopts.optind_opaque
. "$PC_CORE/pc.sh"
pc_begin XCU.getopts.010
pc_test
set -- -c
getopts :c: pcopt 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exit $rc, wanted 0"
[ "$pcopt" = : ] || pc_fail "name is '$pcopt', wanted a colon"
[ "${OPTARG-unset}" = c ] || pc_fail "OPTARG is '${OPTARG-unset}', wanted c"
[ ! -s err ] || pc_fail "diagnostic written despite the leading colon"
pc_pass
