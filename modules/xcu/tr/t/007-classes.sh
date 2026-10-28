# tr understands the character classes: case conversion with lower and upper, and any class when deleting or squeezing.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tr.007
. "$PC_CORE/pc.sh"
pc_begin XCU.tr.007
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
printf 'Hello World 1\n' >in1 || pc_fail "cannot create input"
printf 'a1b22c\n' >in2 || pc_fail "cannot create input"
printf 'a   b\t\tc\n\n\nd\n' >in3 || pc_fail "cannot create input"
printf 'AAbbCc\n' >in4 || pc_fail "cannot create input"
pc_test
got=$(tr '[:lower:]' '[:upper:]' <in1 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "HELLO WORLD 1${NL}rc=0" ] || pc_fail "[:lower:] to [:upper:] gave the wrong result"
got=$(tr '[:upper:]' '[:lower:]' <in1 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "hello world 1${NL}rc=0" ] || pc_fail "[:upper:] to [:lower:] gave the wrong result"
got=$(tr -d '[:digit:]' <in2 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "abc${NL}rc=0" ] || pc_fail "tr -d '[:digit:]' gave the wrong result"
got=$(tr -s '[:space:]' <in3 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a b${T}c${NL}d${NL}rc=0" ] || pc_fail "tr -s '[:space:]' gave the wrong result"
got=$(tr -s '[:upper:]' '[:lower:]' <in4 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "abc${NL}rc=0" ] || pc_fail "tr -s '[:upper:]' '[:lower:]' gave the wrong result"
pc_pass
