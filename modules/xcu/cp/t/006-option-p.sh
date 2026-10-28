# cp -p gives the copy the source modification time and permission bits, regardless of the creation mask.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.006
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.006
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
}

printf 'x\n' >src || pc_fail "cannot create src"
chmod 664 src || pc_fail "cannot chmod src"
touch -t 200101020304 src || pc_fail "cannot set time of src"
pc_test
(umask 077; cp -p src dst) >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cp -p exited with status $rc"
modeof dst || pc_fail "cannot list dst"
[ "$mo" = "-rw-rw-r--" ] || pc_fail "mode is $mo, expected -rw-rw-r--"
[ ! dst -nt src ] || pc_fail "copy is newer than the source"
[ ! src -nt dst ] || pc_fail "copy is older than the source"
pc_pass
