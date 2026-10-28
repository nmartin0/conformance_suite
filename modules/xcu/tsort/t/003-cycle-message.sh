# tsort reports a cycle in its input with a message on standard error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tsort.003
. "$PC_CORE/pc.sh"
pc_begin XCU.tsort.003
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a b\nb a\n' >cyc2 && printf 'a b\nb c\nc a\n' >cyc3 && printf 'a b\nb c\n' >acyc || pc_fail "cannot create input"
pc_test
err=$(tsort cyc2 2>&1 >/dev/null </dev/null)
[ -n "$err" ] || pc_fail "no diagnostic or warning for a two-item cycle"
err=$(tsort cyc3 2>&1 >/dev/null </dev/null)
[ -n "$err" ] || pc_fail "no diagnostic or warning for a three-item cycle"
err=$(tsort acyc 2>&1 >/dev/null </dev/null)
[ -z "$err" ] || pc_fail "a diagnostic was written for input without a cycle"
pc_pass
