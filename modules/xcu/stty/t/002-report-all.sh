# stty -a reports modes, control characters and speed
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.stty.002
#
# Run on a private pseudo-terminal. -a must name each control character as
# "name = value;" and give a speed, and list the mode operands as tokens
# (each present either as itself or with a leading minus).
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.stty.002
pc_pty_need
cat >inner.sh <<'IN'
stty -a >a.out 2>a.err
echo $? >a.rc
IN
pc_test
pc_pty_run inner.sh
[ "$(cat a.rc 2>/dev/null)" = 0 ] || pc_fail "stty -a failed"
[ -s a.out ] || pc_fail "stty -a wrote nothing"
for c in eof eol erase intr kill quit susp start stop; do
	grep "$c = [^;]*;" a.out >/dev/null || pc_fail "no \"$c = value;\" in the -a output"
done
grep 'speed [0-9]* baud;' a.out >/dev/null ||
	grep 'ispeed [0-9]* baud; ospeed [0-9]* baud;' a.out >/dev/null || pc_fail "no speed information"
tr ';' ' ' <a.out >a.words
for m in parenb parodd hupcl cstopb cread clocal ignbrk brkint ignpar parmrk inpck istrip inlcr igncr icrnl ixon ixany ixoff opost isig icanon iexten echo echoe echok echonl noflsh tostop; do
	found=0
	for w in $(cat a.words); do
		[ "$w" = "$m" ] || [ "$w" = "-$m" ] && found=1
	done
	[ "$found" -eq 1 ] || pc_fail "mode $m not listed by -a"
done
cs=0
for w in $(cat a.words); do case $w in cs5|cs6|cs7|cs8) cs=1 ;; esac; done
[ "$cs" -eq 1 ] || pc_fail "no character size listed"
pc_pass
