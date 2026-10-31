# tr -s replaces runs of a repeated character that is in the last operand by one occurrence, after any translation.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tr.003
# MUTANT    tr.sq_all
# MUTANT    tr.sq_none
# MUTANT    tr.sq_two
# MUTANT    tr.sq_mixed
# MUTANT    tr.sq_s1
# MUTANT    tr.sq_before_translate
# MUTANT    tr.s_status1
# MUTANT    tr.no_final_newline
# VARIANT   tr.class_reverse
. "$PC_CORE/pc.sh"
pc_begin XCU.tr.003
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a   b  c\n' >in1 || pc_fail "cannot create input"
printf 'aabbab\n' >in2 || pc_fail "cannot create input"
printf 'abab\n' >in3 || pc_fail "cannot create input"
printf 'aab\n' >in4 || pc_fail "cannot create input"
pc_test
got=$(tr -s ' ' <in1 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a b c${NL}rc=0" ] || pc_fail "tr -s ' ' gave the wrong result"
got=$(tr -s ab xy <in2 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "xyxy${NL}rc=0" ] || pc_fail "tr -s ab xy gave the wrong result"
got=$(tr -s a <in2 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "abbab${NL}rc=0" ] || pc_fail "tr -s a squeezed characters outside its operand"
got=$(tr -s ab xx <in3 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "x${NL}rc=0" ] || pc_fail "tr -s ab xx did not squeeze the translated characters"
got=$(tr -s a x <in4 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "xb${NL}rc=0" ] || pc_fail "tr -s a x did not squeeze by the last operand"
pc_pass
