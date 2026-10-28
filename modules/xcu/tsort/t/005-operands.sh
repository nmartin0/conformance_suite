# tsort reads the file operand or else the standard input, writes nothing for empty input, and fails for a missing file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tsort.005
. "$PC_CORE/pc.sh"
pc_begin XCU.tsort.005
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a b\nb c\n' >in || pc_fail "cannot create input"
: >empty || pc_fail "cannot create input"
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
got=$(tsort in 2>/dev/null </dev/null)
[ "$got" = "a${NL}b${NL}c" ] || pc_fail "a file operand was not read"
got=$(tsort <in 2>/dev/null)
[ "$got" = "a${NL}b${NL}c" ] || pc_fail "standard input was not read"
got=$(printf 'x y\n' | tsort in 2>/dev/null)
[ "$got" = "a${NL}b${NL}c" ] || pc_fail "standard input was read although a file operand was given"
got=$(tsort empty 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "rc=0" ] || pc_fail "empty input did not give no output and status 0"
out=$(tsort nosuchfile 2>/dev/null </dev/null)
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 for a missing file"
[ -z "$out" ] || pc_fail "output was written for a missing file"
pc_pass
