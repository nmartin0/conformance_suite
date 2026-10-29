# A read-only variable gives an exit status above 1.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.read.013
. "$PC_CORE/pc.sh"
pc_begin XCU.read.013
printf 'new\n' >in || pc_fail "cannot write the input"
pc_test
# read must work here, or a failure below could be "read not found"
read pcok <in && [ "$pcok" = new ] || pc_unresolved "read does not set an ordinary variable, so its failure for a read-only one proves nothing"
( readonly pcrd; read pcrd; rc=$?; [ -z "${pcrd+set}" ] || exit 99; exit "$rc" ) <in 2>/dev/null
rc=$?
[ "$rc" -ne 99 ] || pc_fail "read changed a read-only variable"
[ "$rc" -gt 1 ] || pc_fail "exit $rc for a read-only variable, wanted more than 1"
pc_pass
