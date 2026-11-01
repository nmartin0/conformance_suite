# A field after the last one is uninitialized and is not created; fields created between are uninitialized.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.042
# MUTANT    awk.field_read_creates
# MUTANT    awk.fields_always_strings
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.042
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
got=$(echo a | awk '{ print ($2 == 0), ($2 == ""), NF }' 2>/dev/null)
[ "$got" = "1 1 1" ] || pc_fail "a missing field: $got"
got=$(echo 'a b' | awk '{ x = $5; print NF, length($5), $5 + 0, "[" $5 "]" }' 2>/dev/null)
[ "$got" = "2 0 0 []" ] || pc_fail "reading a field beyond NF: $got"
got=$(echo 'a b' | awk '{ $5 = "x"; print ($3 == 0), ($3 == ""), ($4 == 0), ($4 == ""), length($3) }' 2>/dev/null)
[ "$got" = "1 1 1 1 0" ] || pc_fail "the fields created between by an assignment past the last: $got"
pc_pass
