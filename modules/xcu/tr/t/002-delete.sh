# tr -d deletes every input character that is in string1.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tr.002
# MUTANT    tr.d_first_only
# MUTANT    tr.d_inverse
# MUTANT    tr.d_squeeze_rest
# MUTANT    tr.range_literal
# MUTANT    tr.range_excl
# MUTANT    tr.range_start_excl
# MUTANT    tr.d_status1
# MUTANT    tr.no_final_newline
# MUTANT    tr.nul_stops
# VARIANT   tr.class_reverse
. "$PC_CORE/pc.sh"
pc_begin XCU.tr.002
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'education\nabc\n' >in || pc_fail "cannot create input"
printf 'baabb\n' >in2 || pc_fail "cannot create input"
pc_test
got=$(tr -d aeiou <in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "dctn${NL}bc${NL}rc=0" ] || pc_fail "tr -d aeiou gave the wrong result"
got=$(tr -d a-c <in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "edution${NL}${NL}rc=0" ] || pc_fail "tr -d a-c gave the wrong result"
got=$(tr -d a <in2 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "bbb${NL}rc=0" ] || pc_fail "tr -d a left an occurrence of the character"
got=$(printf 'a\000b\n' | tr -d '\000' 2>/dev/null | od -An -v -tx1)
set -- $got
[ "$*" = "61 62 0a" ] || pc_fail "tr -d '\\000' did not delete the null byte only"
pc_pass
