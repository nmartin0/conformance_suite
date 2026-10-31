# tr -c and -C use the complement of string1; -cd deletes everything but string1; -cs squeezes the replacement.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tr.004
# MUTANT    tr.c_ignored
# MUTANT    tr.C_ignored
# MUTANT    tr.c_excl_newline
# MUTANT    tr.c_ascii_only
# MUTANT    tr.cd_inverse
# MUTANT    tr.cs_sq_s1
# MUTANT    tr.c_status1
# MUTANT    tr.C_status1
# MUTANT    tr.d_status1
# MUTANT    tr.no_final_newline
# VARIANT   tr.class_reverse
. "$PC_CORE/pc.sh"
pc_begin XCU.tr.004
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'ab1 2c\n' >in1 || pc_fail "cannot create input"
printf 'a1b22\n' >in2 || pc_fail "cannot create input"
printf 'ab, cd.ef\n' >in3 || pc_fail "cannot create input"
printf 'a\377b\n' >in4 || pc_fail "cannot create input"
pc_test
got=$(tr -c 'a-z\n' '[X*]' <in1 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "abXXXc${NL}rc=0" ] || pc_fail "tr -c 'a-z\\n' '[X*]' gave the wrong result"
got=$(tr -C 'a-z\n' '[X*]' <in1 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "abXXXc${NL}rc=0" ] || pc_fail "tr -C 'a-z\\n' '[X*]' gave the wrong result"
got=$(tr -cd '0-9\n' <in2 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "122${NL}rc=0" ] || pc_fail "tr -cd '0-9\\n' gave the wrong result"
got=$(tr -Cd '0-9\n' <in2 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "122${NL}rc=0" ] || pc_fail "tr -Cd '0-9\\n' gave the wrong result"
got=$(tr -cs 'a-z' '[_*]' <in3 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "ab_cd_ef_rc=0" ] || pc_fail "tr -cs 'a-z' '[_*]' gave the wrong result"
got=$(tr -c a-z '[X*]' <in1 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "abXXXcXrc=0" ] || pc_fail "tr -c a-z '[X*]' did not translate the newline"
got=$(tr -cd 0-9 <in2 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "122rc=0" ] || pc_fail "tr -cd 0-9 did not delete the newline"
got=$(tr -c a '[X*]' <in4 2>/dev/null | od -An -v -tx1)
set -- $got
[ "$*" = "61 58 58 58" ] || pc_fail "tr -c a '[X*]' did not translate a byte above 127"
pc_pass
