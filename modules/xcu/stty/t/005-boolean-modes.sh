# boolean mode operands set and clear the mode
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.stty.005
#
# Input, output (base) and local modes: each X sets the mode (token X in
# -a) and each -X clears it (token -X). Control modes describe hardware a
# line may not have: the operand may fail, or succeed without changing the
# setting (tcsetattr() succeeds when it can make any of the changes), so the
# token is checked only when the operand changed the settings (stty -g).
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.stty.005
pc_pty_need
cat >inner.sh <<'IN'
for m in ignbrk brkint ignpar parmrk inpck istrip inlcr igncr icrnl ixon ixany ixoff opost isig icanon iexten echo echoe echok echonl noflsh tostop; do
	for sign in "" -; do
		stty "$sign$m" 2>/dev/null; echo "$m $sign $?" >>rc.out
		stty -a | tr ';' ' ' >a.$m.$sign.out
	done
done
for m in parenb parodd hupcl cstopb cread clocal; do
	for sign in "" -; do
		g0=$(stty -g)
		if stty "$sign$m" 2>/dev/null; then
			if [ "$(stty -g)" = "$g0" ]; then echo "$m $sign same" >>crc.out
			else echo "$m $sign 0" >>crc.out; fi
			stty -a | tr ';' ' ' >c.$m.$sign.out
		else echo "$m $sign 1" >>crc.out; fi
	done
done
IN
pc_test
pc_pty_run inner.sh
has() { for w in $(cat "$1"); do [ "$w" = "$2" ] && return 0; done; return 1; }
for m in ignbrk brkint ignpar parmrk inpck istrip inlcr igncr icrnl ixon ixany ixoff opost isig icanon iexten echo echoe echok echonl noflsh tostop; do
	grep "^$m  0$" rc.out >/dev/null || pc_fail "stty $m failed"
	grep "^$m - 0$" rc.out >/dev/null || pc_fail "stty -$m failed"
	has "a.$m..out" "$m" || pc_fail "after stty $m the -a output lacks $m"
	has "a.$m.-.out" "-$m" || pc_fail "after stty -$m the -a output lacks -$m"
done
for m in parenb parodd hupcl cstopb cread clocal; do
	if grep "^$m  0$" crc.out >/dev/null; then has "c.$m..out" "$m" || pc_fail "after stty $m the -a output lacks $m"; fi
	if grep "^$m - 0$" crc.out >/dev/null; then has "c.$m.-.out" "-$m" || pc_fail "after stty -$m the -a output lacks -$m"; fi
	for sign in "" -; do
		if grep "^$m $sign same$" crc.out >/dev/null; then
			has "c.$m.$sign.out" "$m" || has "c.$m.$sign.out" "-$m" || pc_fail "after stty $sign$m the -a output shows neither $m nor -$m"
		fi
	done
done
pc_pass
