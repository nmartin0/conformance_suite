# A read-only variable gives an exit status above 1.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.read.013
. "$PC_CORE/pc.sh"
pc_begin XCU.read.013
printf 'new\n' >in || pc_fail "cannot write the input"
pc_test
( readonly pcrd; read pcrd ) <in 2>/dev/null
rc=$?
[ "$rc" -gt 1 ] || pc_fail "exit $rc for a read-only variable, wanted more than 1"
pc_pass
