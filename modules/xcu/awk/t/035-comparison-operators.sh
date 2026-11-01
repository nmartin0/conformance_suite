# comparison operators
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.035
# MUTANT    awk.le_as_lt
# MUTANT    awk.ge_as_gt
# MUTANT    awk.lt_as_le
# MUTANT    awk.gt_as_ge
# MUTANT    awk.ne_as_eq
# MUTANT    awk.const_digits_numeric
# MUTANT    awk.fields_always_strings
# <, <=, >, >=, == and != compare numbers numerically and strings by
# collation, a string constant forcing a string comparison, and numeric
# strings from input numerically.
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.035
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
got=$(awk 'BEGIN { print (1 <= 1), (1 <= 0), (2 >= 2), (1 >= 2), (1 != 2), (1 != 1), (1 < 2), (2 > 1), ("a" < "b"), ("b" <= "a"), ("abc" != "abd"), ("10" < "9"), (10 < 9), (1 < 1), (1 > 1), ("a" < "a"), ("b" > "b") }' 2>/dev/null </dev/null)
[ "$got" = "1 0 1 0 1 0 1 1 1 0 1 1 0 0 0 0 0" ] || pc_fail "comparisons of constants: $got"
got=$(echo '10 9 abc abd' | awk '{ print ($1 <= $2), ($1 >= $2), ($1 != $2), ($3 < $4), ($3 >= $4), ($1 < $3) }' 2>/dev/null)
[ "$got" = "0 1 1 1 0 1" ] || pc_fail "comparisons of fields: $got"
pc_pass
