# With more than two operands and a last operand that is not a directory, ln reports an error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ln.012
# MUTANT    ln.many_ignores_extra
# MUTANT    ln.err_silent
# MUTANT    ln.err_status0
# VARIANT   ln.status2
# VARIANT   ln.status255
. "$PC_CORE/pc.sh"
pc_begin XCU.ln.012
printf '1\n' >a || pc_fail "cannot create a"
printf '2\n' >b || pc_fail "cannot create b"
printf '3\n' >b2 || pc_fail "cannot create b2"
pc_test
ln a n1 n2 >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "ln exited 0 with three operands and no target directory"
[ -s err ] || pc_fail "no diagnostic for three operands and no target directory"
ln a b b2 >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "ln exited 0 with three operands and a regular file last"
[ -s err ] || pc_fail "no diagnostic for three operands and a regular file last"
pc_pass
