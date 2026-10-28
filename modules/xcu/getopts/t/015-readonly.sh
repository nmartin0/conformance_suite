# A read-only name variable gives an exit status above 1.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getopts.015
. "$PC_CORE/pc.sh"
pc_begin XCU.getopts.015
pc_test
( readonly pcro; set -- -a; getopts ab pcro ) 2>/dev/null
rc=$?
[ "$rc" -gt 1 ] || pc_fail "exit $rc for a read-only name variable, wanted more than 1"
pc_pass
