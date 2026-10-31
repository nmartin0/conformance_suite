# uniq -f n ignores the first n fields (blanks plus non-blanks) in comparisons.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uniq.005
# MUTANT    uniq.f_ignored
# MUTANT    uniq.f_plus1
# MUTANT    uniq.f_minus1
# MUTANT    uniq.f_chars
# MUTANT    uniq.f_trailing
# MUTANT    uniq.f_spaceonly
# MUTANT    uniq.f_single
# MUTANT    uniq.keep_last
# MUTANT    uniq.no_newline
# MUTANT    uniq.status1
. "$PC_CORE/pc.sh"
pc_begin XCU.uniq.005
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'k1 same x\nk2 same x\nk3 other x\n' >in1 || pc_fail "cannot create input"
printf '  k1 same\n k2 same\nk3 same\n' >in2 || pc_fail "cannot create input"
printf 'k1  same\nk2 same\n' >in3 || pc_fail "cannot create input"
printf 'k1\tsame\nk2\tsame\nk3\tother\n' >in4 || pc_fail "cannot create input"
printf 'p x\nq x\n' >in5 || pc_fail "cannot create input"
pc_test
got=$(uniq -f 1 in1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "k1 same x${NL}k3 other x${NL}rc=0" ] || pc_fail "-f 1 did not skip the first field"
got=$(uniq -f 1 in2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "  k1 same${NL}rc=0" ] || pc_fail "-f 1 did not count leading blanks as part of a field"
got=$(uniq -f 2 in1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "k1 same x${NL}rc=0" ] || pc_fail "-f 2 did not skip two fields"
got=$(uniq -f 1 in3 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "k1  same${NL}k2 same${NL}rc=0" ] || pc_fail "-f 1 did not leave the blanks after the first field in the comparison"
got=$(uniq -f 1 in4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
T=$(printf '\t')
[ "$got" = "k1${T}same${NL}k3${T}other${NL}rc=0" ] || pc_fail "-f 1 did not treat a tab as a blank"
got=$(uniq -f 1 in5 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "p x${NL}rc=0" ] || pc_fail "-f 1 did not ignore the first field of a short line"
pc_pass
