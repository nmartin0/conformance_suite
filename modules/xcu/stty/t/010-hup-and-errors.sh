# hup is equivalent to hupcl; unknown operands are errors
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.stty.010
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.stty.010
pc_pty_need
cat >inner.sh <<'IN'
if stty hupcl 2>/dev/null; then
	stty -g >h1.out; stty -hupcl; stty hup; echo "hup $?" >rc.out; stty -g >h2.out
	stty -hup; echo "-hup $?" >>rc.out; stty -g >h3.out
	stty hupcl; stty -hupcl; stty -g >h4.out
else
	echo none >h1.out
fi
stty no-such-operand-pc >bad.out 2>bad.err; echo $? >bad.rc
stty -Q >badopt.out 2>badopt.err; echo $? >badopt.rc
IN
pc_test
pc_pty_run inner.sh
if [ -s h2.out ]; then
	cmp -s h1.out h2.out || pc_fail "hup did not behave as hupcl"
	cmp -s h3.out h4.out || pc_fail "-hup did not behave as -hupcl"
fi
[ "$(cat bad.rc)" -gt 0 ] || pc_fail "an unknown operand was accepted"
[ "$(cat badopt.rc)" -gt 0 ] || pc_fail "an unknown option was accepted"
[ ! -s bad.out ] || pc_fail "an unknown operand wrote to standard output"
pc_pass
