# If one operand cannot be touched, touch diagnoses it, handles the others, and exits non-zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# The operand that cannot be touched is in a directory that does not
# exist. It comes first, last and between others.
#
# ASSERTION XCU.touch.012
# MUTANT    touch.err_no_diag
# MUTANT    touch.err_stdout
# MUTANT    touch.err_status0
# MUTANT    touch.err_stops
# MUTANT    touch.err_status_last
# MUTANT    touch.err_status_first
# VARIANT   touch.err_status_3
. "$PC_CORE/pc.sh"
pc_begin XCU.touch.012
[ ! -e nodir ] || pc_fail "nodir exists"
touch -t 200101010101 a1 a2 b1 b2 c1 c2 || pc_fail "cannot create the files"
touch -t 200101010101 ref || pc_fail "cannot create ref"
pc_test
touch nodir/x a1 a2 >out1 2>err1 </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "bad operand first: touch exited 0"
[ -s err1 ] || pc_fail "bad operand first: no diagnostic"
[ ! -s out1 ] || pc_fail "bad operand first: the diagnostic went to standard output"
[ a1 -nt ref ] && [ a2 -nt ref ] || pc_fail "bad operand first: the others were not touched"
touch b1 b2 nodir/y >out2 2>err2 </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "bad operand last: touch exited 0"
[ -s err2 ] || pc_fail "bad operand last: no diagnostic"
[ ! -s out2 ] || pc_fail "bad operand last: the diagnostic went to standard output"
[ b1 -nt ref ] && [ b2 -nt ref ] || pc_fail "bad operand last: the others were not touched"
touch c1 nodir/z c2 f >out3 2>err3 </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "bad operand between: touch exited 0"
[ -s err3 ] || pc_fail "bad operand between: no diagnostic"
[ c1 -nt ref ] && [ c2 -nt ref ] || pc_fail "bad operand between: the others were not touched"
[ -f f ] || pc_fail "bad operand between: the missing file after it was not created"
pc_pass
