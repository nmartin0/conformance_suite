# tr -s replaces runs of a repeated character that is in the last operand by one occurrence, after any translation.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tr.003
. "$PC_CORE/pc.sh"
pc_begin XCU.tr.003
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a   b  c\n' >in1 || pc_fail "cannot create input"
printf 'aabbab\n' >in2 || pc_fail "cannot create input"
pc_test
got=$(tr -s ' ' <in1 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a b c${NL}rc=0" ] || pc_fail "tr -s ' ' gave the wrong result"
got=$(tr -s ab xy <in2 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "xyxy${NL}rc=0" ] || pc_fail "tr -s ab xy gave the wrong result"
got=$(tr -s a <in2 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "abbab${NL}rc=0" ] || pc_fail "tr -s a squeezed characters outside its operand"
pc_pass
