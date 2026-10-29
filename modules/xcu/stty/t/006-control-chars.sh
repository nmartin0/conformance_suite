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
# The checks use only the shell (no grep or cmp): a line of FILE that equals
# or contains a string, and two one-line files that are equal.
line_is() { while IFS= read -r l; do [ "$l" = "$2" ] && return 0; done <"$1" 2>/dev/null; return 1; }
line_has() { while IFS= read -r l; do case $l in *"$2"*) return 0 ;; esac; done <"$1" 2>/dev/null; return 1; }
same_first_line() {
	a= b=
	IFS= read -r a <"$1" 2>/dev/null
	IFS= read -r b <"$2" 2>/dev/null
	[ -n "$a" ] && [ "$a" = "$b" ]
}
pc_test
pc_pty_run inner.sh
for n in eof eol erase intr kill quit susp start stop; do
	line_is rc.out "$n x 0" || pc_fail "stty $n x failed"
	line_has "one.$n.out" "$n = x;" || pc_fail "after stty $n x the -a output does not show $n = x;"
	line_is rc.out "$n undef 0" || pc_fail "stty $n undef failed"
	line_has "undef.$n.out" "$n = <undef>;" || pc_fail "after stty $n undef the output does not show <undef>"
	line_is rc.out "$n caret 0" || pc_fail "stty $n ^- failed"
	line_has "caret.$n.out" "$n = <undef>;" || pc_fail "after stty $n ^- the output does not show <undef>"
done
line_is rc.out "ctl 0" || pc_fail "stty intr ^A failed"
line_is rc.out "lit 0" || pc_fail "stty intr <SOH> failed"
same_first_line g1.out g2.out || pc_fail "intr ^A and intr <SOH> give different settings"
line_is rc.out "lower 0" || pc_fail "stty intr ^a failed"
same_first_line g1.out g3.out || pc_fail "intr ^a and intr ^A give different settings"
pc_pass
