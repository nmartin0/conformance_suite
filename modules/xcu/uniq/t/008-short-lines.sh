# When -f or -s asks to skip more than a line holds, the line is compared as an empty string.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uniq.008
. "$PC_CORE/pc.sh"
pc_begin XCU.uniq.008
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a b\nc d\n' >in1 || pc_fail "cannot create input"
printf 'ab\ncd\n' >in2 || pc_fail "cannot create input"
pc_test
got=$(uniq -f 5 in1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a b${NL}rc=0" ] || pc_fail "-f beyond the fields did not make the lines equal"
got=$(uniq -s 9 in2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "ab${NL}rc=0" ] || pc_fail "-s beyond the line did not make the lines equal"
pc_pass
