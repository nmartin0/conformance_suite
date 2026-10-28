# An unknown option gives ? , unset OPTARG, a diagnostic and exit 0.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getopts.007
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
