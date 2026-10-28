# chmod with an octal mode sets the permission bits absolutely, for each file operand.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chmod.001
. "$PC_CORE/pc.sh"
pc_begin XCU.chmod.001
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
}

for n in f g; do
	: >"$n" || pc_fail "cannot create $n"
	chmod 777 "$n" || pc_fail "cannot chmod $n"
done
pc_test
chmod 640 f >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "chmod 640 exited with status $rc"
modeof f || pc_fail "cannot list f"
[ "$mo" = "-rw-r-----" ] || pc_fail "mode is $mo, expected -rw-r-----"
chmod 0600 f g >out 2>err </dev/null
modeof f || pc_fail "cannot list f"
[ "$mo" = "-rw-------" ] || pc_fail "f: mode is $mo, expected -rw-------"
modeof g || pc_fail "cannot list g"
[ "$mo" = "-rw-------" ] || pc_fail "g: mode is $mo, expected -rw-------"
[ ! -s out ] || pc_fail "chmod wrote to standard output"
pc_pass
