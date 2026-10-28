# In tr operands, octal sequences and the C escape sequences stand for single characters, and octal range ends give a range of coded values.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tr.006
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
pc_pass
