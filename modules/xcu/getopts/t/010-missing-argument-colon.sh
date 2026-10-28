# With a leading colon a missing option-argument gives : and OPTARG.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getopts.010
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
