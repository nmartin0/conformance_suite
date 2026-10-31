# In tr operands, octal sequences and the C escape sequences stand for single characters, and octal range ends give a range of coded values.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tr.006
# MUTANT    tr.esc_a
# MUTANT    tr.esc_b
# MUTANT    tr.esc_f
# MUTANT    tr.esc_n
# MUTANT    tr.esc_r
# MUTANT    tr.esc_t
# MUTANT    tr.esc_v
# MUTANT    tr.oct_one_digit
# MUTANT    tr.oct_decimal
# MUTANT    tr.oct_long
# MUTANT    tr.oct_need3
# MUTANT    tr.oct_range_literal
# MUTANT    tr.bs_bs_n
# MUTANT    tr.no_final_newline
# VARIANT   tr.pad_last
# VARIANT   tr.dup_last
. "$PC_CORE/pc.sh"
pc_begin XCU.tr.006
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
printf 'a%sb\n' "$T" >in1 || pc_fail "cannot create input"
printf 'banana\n' >in2 || pc_fail "cannot create input"
printf 'a\\b\n' >in3 || pc_fail "cannot create input"
printf 'xay\n' >in4 || pc_fail "cannot create input"
printf 'ABCD\n' >in5 || pc_fail "cannot create input"
pc_test
got=$(tr '\t' ' ' <in1 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a b${NL}rc=0" ] || pc_fail "tr '\\t' ' ' gave the wrong result"
got=$(tr '\141' z <in2 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "bznznz${NL}rc=0" ] || pc_fail "tr '\\141' z gave the wrong result"
got=$(tr '\\' '/' <in3 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a/b${NL}rc=0" ] || pc_fail "tr '\\\\' '/' gave the wrong result"
got=$(tr a '\n' <in4 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "x${NL}y${NL}rc=0" ] || pc_fail "tr a '\\n' gave the wrong result"
got=$(tr '\101-\103' xyz <in5 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "xyzD${NL}rc=0" ] || pc_fail "tr '\\101-\\103' xyz gave the wrong result"
# every backslash escape names its own character
got=$(printf '\a\b\f\n\r\t\v\\' | tr '\a\b\f\n\r\t\v\\' '12345678' 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "12345678rc=0" ] || pc_fail "the backslash escapes did not stand for their characters"
# an octal sequence of one or two digits, and at most three digits
printf '0a\n' >in6 || pc_fail "cannot create input"
printf 'a1b\n' >in7 || pc_fail "cannot create input"
printf 'a\\n\n' >in8 || pc_fail "cannot create input"
got=$(tr '\60' x <in6 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "xa${NL}rc=0" ] || pc_fail "tr '\\60' x gave the wrong result"
got=$(tr '\1411' xy <in7 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "xyb${NL}rc=0" ] || pc_fail "an octal sequence took more than three digits"
got=$(tr 'A-\103' xyz <in5 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "xyzD${NL}rc=0" ] || pc_fail "tr 'A-\\103' xyz gave the wrong result"
got=$(tr '\\n' xy <in8 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "axy${NL}rc=0" ] || pc_fail "tr '\\\\n' xy gave the wrong result"
pc_pass
