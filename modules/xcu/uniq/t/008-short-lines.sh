# When -f or -s asks to skip more than a line holds, the line is compared as an empty string.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uniq.008
# MUTANT    uniq.short_f_whole
# MUTANT    uniq.short_s_whole
# MUTANT    uniq.s_ge_whole
# MUTANT    uniq.short_f_error
# MUTANT    uniq.short_s_error
# MUTANT    uniq.f_ignored
# MUTANT    uniq.s_ignored
# MUTANT    uniq.status1
. "$PC_CORE/pc.sh"
pc_begin XCU.uniq.008
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a b\nc d\n' >in1 || pc_fail "cannot create input"
printf 'ab\ncd\n' >in2 || pc_fail "cannot create input"
printf 'a b\nc d\n' >in3 || pc_fail "cannot create input"
printf 'a bc\nd ef\n' >in4 || pc_fail "cannot create input"
pc_test
got=$(uniq -f 5 in1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a b${NL}rc=0" ] || pc_fail "-f beyond the fields did not make the lines equal"
got=$(uniq -s 9 in2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "ab${NL}rc=0" ] || pc_fail "-s beyond the line did not make the lines equal"
got=$(uniq -f 2 in3 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a b${NL}rc=0" ] || pc_fail "-f equal to the number of fields did not make the lines equal"
got=$(uniq -s 2 in2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "ab${NL}rc=0" ] || pc_fail "-s equal to the line length did not make the lines equal"
got=$(uniq -f 1 -s 5 in4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a bc${NL}rc=0" ] || pc_fail "-s beyond what remains after -f did not make the lines equal"
pc_pass
