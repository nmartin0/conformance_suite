# XSI raw and cooked
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.stty.012
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.stty.012
pc_pty_need
cat >inner.sh <<'IN'
stty raw; echo "raw $?" >rc.out
stty -a | tr ';' ' ' >raw.out
stty -a >raw.out2
stty sane; stty -raw; echo "-raw $?" >>rc.out
stty -a | tr ';' ' ' >cooked.out
stty sane; stty cooked; echo "cooked $?" >>rc.out
stty -a | tr ';' ' ' >cooked2.out
IN
pc_test
pc_pty_run inner.sh
has() { for w in $(cat "$1"); do [ "$w" = "$2" ] && return 0; done; return 1; }
grep '^raw 0$' rc.out >/dev/null || pc_fail "stty raw failed"
has raw.out cs8 && has raw.out -opost && has raw.out -inpck || pc_fail "raw did not give cs8 -opost -inpck"
for n in erase kill intr quit eof eol; do
	grep "$n = <undef>;" raw.out2 >/dev/null || pc_fail "raw left $n enabled"
done
grep '^-raw 0$' rc.out >/dev/null || pc_fail "stty -raw failed"
grep '^cooked 0$' rc.out >/dev/null || pc_fail "stty cooked failed"
cmp -s cooked.out cooked2.out || pc_fail "-raw and cooked differ"
pc_pass
