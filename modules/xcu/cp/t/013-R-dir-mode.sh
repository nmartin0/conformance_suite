# cp -R makes copied directories usable while it fills them and leaves them with the source permissions (modified by the mask): a read-only directory with content is copied with its mode.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.013
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.013
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
}

trap 'chmod -R u+rwx src dst 2>/dev/null' EXIT
mkdir src || pc_fail "cannot create src"
printf 'x\n' >src/f || pc_fail "cannot create src/f"
chmod 500 src || pc_fail "cannot chmod src"
pc_test
(umask 022; cp -R src dst) >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cp -R exited with status $rc"
[ -f dst/f ] || pc_fail "dst/f was not copied"
modeof dst || pc_fail "cannot list dst"
[ "$mo" = "dr-x------" ] || pc_fail "dst has mode $mo, expected dr-x------"
pc_pass
