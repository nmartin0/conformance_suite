# uniq -s n ignores the first n characters in comparisons, counted after any fields skipped by -f.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uniq.006
# MUTANT    uniq.s_ignored
# MUTANT    uniq.s_plus1
# MUTANT    uniq.s_minus1
# MUTANT    uniq.s_nonblank
# MUTANT    uniq.s_before_f
# MUTANT    uniq.s_ignored_with_f
# MUTANT    uniq.f_ignored_with_s
# MUTANT    uniq.f_s_as_fields
# MUTANT    uniq.keep_last
# MUTANT    uniq.no_newline
# MUTANT    uniq.status1
. "$PC_CORE/pc.sh"
pc_begin XCU.uniq.006
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'xxabc\nyyabc\nzzabd\n' >in1 || pc_fail "cannot create input"
printf 'k1 Xabc\nk2 Yabc\nk3 Xabd\n' >in2 || pc_fail "cannot create input"
printf ' ab\n ac\n' >in3 || pc_fail "cannot create input"
pc_test
got=$(uniq -s 2 in1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "xxabc${NL}zzabd${NL}rc=0" ] || pc_fail "-s 2 did not skip two characters"
got=$(uniq -f 1 -s 2 in2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "k1 Xabc${NL}k3 Xabd${NL}rc=0" ] || pc_fail "-f 1 -s 2 did not skip a field and then two characters"
got=$(uniq -s 2 in3 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = " ab${NL} ac${NL}rc=0" ] || pc_fail "-s 2 did not count a blank as a character"
got=$(uniq -s 1 in3 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = " ab${NL} ac${NL}rc=0" ] || pc_fail "-s 1 did not leave the rest of the line in the comparison"
pc_pass
