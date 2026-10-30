# g, G, h, H and x copy, append and exchange the pattern and hold spaces.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sed.009
. "$PC_CORE/pc.sh"
pc_begin XCU.sed.009
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '1\n2\n3\n4\n' >in4 || pc_fail "cannot create input"
printf 'a\nb\nc\n' >in3 || pc_fail "cannot create input"
pc_test
got=$(sed -n 'h;n;G;p' in4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "2${NL}1${NL}4${NL}3${NL}rc=0" ] || pc_fail "h;n;G;p gave the wrong result"
got=$(sed x in3 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "${NL}a${NL}b${NL}rc=0" ] || pc_fail "x gave the wrong result (hold space starts empty)"
got=$(sed -n 'H;${x;p;}' in3 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "${NL}a${NL}b${NL}c${NL}rc=0" ] || pc_fail "H then x did not give an empty line followed by the lines"
got=$(sed -n '1h;3{g;p;}' in3 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}rc=0" ] || pc_fail "1h then g gave the wrong result"
got=$(sed 'G' in3 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}${NL}b${NL}${NL}c${NL}${NL}rc=0" ] || pc_fail "G with an empty hold space did not add an empty line"
got=$(sed -n '1!G;h;$p' in3 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "c${NL}b${NL}a${NL}rc=0" ] || pc_fail "the line-reversing idiom failed"
pc_pass
