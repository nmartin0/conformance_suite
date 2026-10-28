# The null utility supports no options and does not treat "--" specially:
# option-like operands and "--" are accepted without a diagnostic and with
# status zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.colon.002
. "$PC_CORE/pc.sh"
pc_begin XCU.colon.002
pc_test
for arg in -x -- -zq --help +o -; do
	: "$arg" 2>pc.err
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "colon $arg exited with status $rc"
	[ -s pc.err ] && pc_fail "colon $arg wrote to standard error"
done
: -- x -a 2>pc.err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "colon -- x -a exited with status $rc"
[ -s pc.err ] && pc_fail "colon -- x -a wrote to standard error"
pc_pass
