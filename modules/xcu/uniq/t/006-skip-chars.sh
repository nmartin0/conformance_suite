# uniq -s n ignores the first n characters in comparisons, counted after any fields skipped by -f.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uniq.006
. "$PC_CORE/pc.sh"
pc_begin XCU.uniq.006
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'xxabc\nyyabc\nzzabd\n' >in1 || pc_fail "cannot create input"
printf 'k1 Xabc\nk2 Yabc\nk3 Xabd\n' >in2 || pc_fail "cannot create input"
pc_test
got=$(uniq -s 2 in1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "xxabc${NL}zzabd${NL}rc=0" ] || pc_fail "-s 2 did not skip two characters"
got=$(uniq -f 1 -s 2 in2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "k1 Xabc${NL}k3 Xabd${NL}rc=0" ] || pc_fail "-f 1 -s 2 did not skip a field and then two characters"
pc_pass
