# With a leading colon an unknown option sets OPTARG and is silent.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getopts.008
# MUTANT    getopts.ucolon_name_colon
# MUTANT    getopts.ucolon_optarg_unset
# MUTANT    getopts.ucolon_optarg_q
# MUTANT    getopts.ucolon_diag
# MUTANT    getopts.ucolon_status1
# VARIANT   getopts.reimpl
# VARIANT   getopts.optind_opaque
. "$PC_CORE/pc.sh"
pc_begin XCU.getopts.008
pc_test
set -- -Q
getopts :ab pcopt 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exit $rc, wanted 0"
[ "$pcopt" = '?' ] || pc_fail "name is '$pcopt', wanted ?"
[ "${OPTARG-unset}" = Q ] || pc_fail "OPTARG is '${OPTARG-unset}', wanted Q"
[ ! -s err ] || pc_fail "diagnostic written despite the leading colon"
pc_pass
