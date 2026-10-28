# If one file cannot be changed, chmod diagnoses it, changes the others, and exits non-zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chmod.010
. "$PC_CORE/pc.sh"
pc_begin XCU.chmod.010
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
}

: >f || pc_fail "cannot create f"
chmod 644 f || pc_fail "cannot chmod f"
pc_test
chmod 600 nonexistent f >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "chmod exited 0 although a file does not exist"
[ -s err ] || pc_fail "no diagnostic"
modeof f || pc_fail "cannot list f"
[ "$mo" = "-rw-------" ] || pc_fail "f has mode $mo, expected -rw-------"
pc_pass
