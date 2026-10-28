# With more than two operands and a last operand that is not a directory, ln reports an error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ln.012
. "$PC_CORE/pc.sh"
pc_begin XCU.ln.012
printf '1\n' >a || pc_fail "cannot create a"
printf '2\n' >b || pc_fail "cannot create b"
pc_test
ln a b c >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "ln exited 0 with three operands and no target directory"
pc_pass
