# One clause may carry several actions, applied in order.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chmod.004
. "$PC_CORE/pc.sh"
pc_begin XCU.chmod.004
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
}

: >f || pc_fail "cannot create f"
chmod 000 f || pc_fail "cannot chmod f"
pc_test
chmod u+rwx-w f >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "chmod u+rwx-w exited with status $rc"
modeof f || pc_fail "cannot list f"
[ "$mo" = "-r-x------" ] || pc_fail "mode is $mo, expected -r-x------"
pc_pass
