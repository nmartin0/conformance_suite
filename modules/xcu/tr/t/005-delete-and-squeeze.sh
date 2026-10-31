# tr -d -s deletes the characters of string1 and then squeezes runs of characters of string2.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tr.005
# MUTANT    tr.ds_no_delete
# MUTANT    tr.sq_none
# MUTANT    tr.ds_swap
# MUTANT    tr.ds_sq_s1
# MUTANT    tr.ds_sq_before_delete
# MUTANT    tr.s2_class_literal
# MUTANT    tr.no_final_newline
# VARIANT   tr.class_reverse
. "$PC_CORE/pc.sh"
pc_begin XCU.tr.005
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'axx  b\n' >in || pc_fail "cannot create input"
pc_test
got=$(tr -ds x ' ' <in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a b${NL}rc=0" ] || pc_fail "tr -ds x ' ' gave the wrong result"
got=$(printf 'axa\n' | tr -ds x a 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}rc=0" ] || pc_fail "tr -ds x a did not squeeze after deleting"
got=$(printf 'a11b22\n' | tr -ds a '[:digit:]' 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "1b2${NL}rc=0" ] || pc_fail "tr -ds a '[:digit:]' gave the wrong result"
pc_pass
