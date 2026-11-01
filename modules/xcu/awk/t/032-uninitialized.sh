# uninitialized values
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.032
# MUTANT    awk.uninit_var_zero_only
# MUTANT    awk.uninit_elem_zero_only
# MUTANT    awk.fn_arg_zero
# MUTANT    awk.past_end_no_extend
# An unset variable or array element is 0 as a number and
# the empty string as a string;
# assigning past the end creates empty fields.
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.032
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
got=$(awk 'BEGIN { print (x == 0), (x == ""), length(x), (a["k"] == 0), (a["k"] == ""), x + 0, "[" x "]" }' 2>/dev/null </dev/null)
[ "$got" = "1 1 0 1 1 0 []" ] || pc_fail "unset variable or element: $got"
got=$(echo 'a b' | awk '{ $5 = "x"; print NF, $0 }' 2>/dev/null)
[ "$got" = "5 a b   x" ] || pc_fail "assigning past the last field: $got"
got=$(awk 'function f(p) { return (p == 0) (p == "") } BEGIN { print f() }' 2>/dev/null </dev/null)
[ "$got" = "11" ] || pc_fail "a missing function argument: $got"
pc_pass
