# which text counts as a numeric string
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.027
# Field text counts as a number when it reads as a decimal number with an
# optional sign, optional blanks around it, a period or an exponent, and as a
# string otherwise.
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.027
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
got=$(echo '+5 5 5x 5' | awk '{ print ($1 == $2), ($3 == $4), ($3 < 6) }' 2>/dev/null)
[ "$got" = "1 0 1" ] || pc_fail "sign and trailing letters: $got"
got=$(echo '.5 0.5 5. 5 +.5e1 5 -3 -3.0' | awk '{ print ($1 == $2), ($3 == $4), ($5 == $6), ($7 == $8) }' 2>/dev/null)
[ "$got" = "1 1 1 1" ] || pc_fail "decimal forms: $got"
got=$(echo '1 2' | awk '{ print ($0 == 1), ($0 == "1 2") }' 2>/dev/null)
[ "$got" = "0 1" ] || pc_fail "a record with two numbers was taken as a number: $got"
got=$(echo ' 7 ' | awk 'BEGIN { FS = "," } { print ($1 == 7), ($0 == 7) }' 2>/dev/null)
[ "$got" = "1 1" ] || pc_fail "blanks around a numeric field or record: $got"
pc_pass
