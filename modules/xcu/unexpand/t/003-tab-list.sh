# unexpand -t uses the given tab stops, converts blanks anywhere on the line, and converts nothing beyond the last stop of a list.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.unexpand.003
. "$PC_CORE/pc.sh"
pc_begin XCU.unexpand.003
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
printf 'a   b\n' >in1 && printf '    x\n' >in2 && printf '        x\n' >in3 && printf '            x\n' >in4 && printf 'ab  cd\n' >in5 || pc_fail "cannot create input"
pc_test
got=$(unexpand -t 4 in1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${T}b${NL}rc=0" ] || pc_fail "-t 4 did not convert blanks inside the line"
got=$(unexpand -t 4 in2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "${T}x${NL}rc=0" ] || pc_fail "-t 4 did not convert four leading spaces"
got=$(unexpand -t 4 in3 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "${T}${T}x${NL}rc=0" ] || pc_fail "-t 4 did not convert eight leading spaces"
got=$(unexpand -t 4 in5 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "ab${T}cd${NL}rc=0" ] || pc_fail "-t 4 did not convert two spaces ending at column 4"
got=$(unexpand -t 4,8 in4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "${T}${T}    x${NL}rc=0" ] || pc_fail "-t 4,8 converted blanks beyond the last tab stop"
pc_pass
