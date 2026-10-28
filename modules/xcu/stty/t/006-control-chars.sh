# character assignments: single character, undef and ^-
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.stty.006
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.stty.006
pc_pty_need
cat >inner.sh <<'IN'
for n in eof eol erase intr kill quit susp start stop; do
	stty "$n" x 2>/dev/null; echo "$n x $?" >>rc.out
	stty -a >one.$n.out
	stty "$n" undef 2>/dev/null; echo "$n undef $?" >>rc.out
	stty -a >undef.$n.out
	stty "$n" x; stty "$n" '^-' 2>/dev/null; echo "$n caret $?" >>rc.out
	stty -a >caret.$n.out
done
stty intr '^A'; echo "ctl $?" >>rc.out
stty -g >g1.out
stty intr "`printf '\001'`"; echo "lit $?" >>rc.out
stty -g >g2.out
stty intr '^a'; echo "lower $?" >>rc.out
stty -g >g3.out
IN
pc_test
pc_pty_run inner.sh
for n in eof eol erase intr kill quit susp start stop; do
	grep "^$n x 0$" rc.out >/dev/null || pc_fail "stty $n x failed"
	grep "$n = x;" "one.$n.out" >/dev/null || pc_fail "after stty $n x the -a output does not show $n = x;"
	grep "^$n undef 0$" rc.out >/dev/null || pc_fail "stty $n undef failed"
	grep "$n = <undef>;" "undef.$n.out" >/dev/null || pc_fail "after stty $n undef the output does not show <undef>"
	grep "^$n caret 0$" rc.out >/dev/null || pc_fail "stty $n ^- failed"
	grep "$n = <undef>;" "caret.$n.out" >/dev/null || pc_fail "after stty $n ^- the output does not show <undef>"
done
grep '^ctl 0$' rc.out >/dev/null || pc_fail "stty intr ^A failed"
grep '^lit 0$' rc.out >/dev/null || pc_fail "stty intr <SOH> failed"
cmp -s g1.out g2.out || pc_fail "intr ^A and intr <SOH> give different settings"
grep '^lower 0$' rc.out >/dev/null || pc_fail "stty intr ^a failed"
cmp -s g1.out g3.out || pc_fail "intr ^a and intr ^A give different settings"
pc_pass
