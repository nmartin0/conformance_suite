# d deletes and restarts, n and N bring in the next line, P writes up to a newline, and D deletes up to a newline and restarts without reading.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sed.008
. "$PC_CORE/pc.sh"
pc_begin XCU.sed.008
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '1\n2\n3\n4\n' >in4 && printf 'a\nb\nc\nd\ne\n' >in5 || pc_fail "cannot create input"
pc_test
got=$(sed '2d' in4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}3${NL}4${NL}rc=0" ] || pc_fail "d gave the wrong lines"
got=$(sed 'd;p' in4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "rc=0" ] || pc_fail "commands after d were executed or output was written"
got=$(sed 'n;d' in4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}3${NL}rc=0" ] || pc_fail "n;d gave the wrong lines"
got=$(sed -n 'n;p' in4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "2${NL}4${NL}rc=0" ] || pc_fail "-n with n;p gave the wrong lines"
got=$(sed 'N;s/\n/+/' in4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1+2${NL}3+4${NL}rc=0" ] || pc_fail "N;s gave the wrong lines"
got=$(sed -n 'N;P' in4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}3${NL}rc=0" ] || pc_fail "N;P did not write only the text before the newline"
got=$(sed -n 'N;N;P;D' in5 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}b${NL}rc=0" ] || pc_fail "D did not restart with the rest of the pattern space and without reading"
got=$(sed '$!N;$!D' in4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "3${NL}4${NL}rc=0" ] || pc_fail "the last-two-lines idiom with D failed"
got=$(sed -n -e '$!{h;d}' -e 'x;G;p' in4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "3${NL}4${NL}rc=0" ] || pc_fail "braces with ! and d failed"
pc_pass
