# A newly created copy gets the source permission bits modified by the file creation mask.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.005
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.005
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
}

printf 'x\n' >src || pc_fail "cannot create src"
chmod 777 src || pc_fail "cannot chmod src"
pc_test
(umask 022; cp src dst1) >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cp exited with status $rc"
modeof dst1 || pc_fail "cannot list dst1"
[ "$mo" = "-rwxr-xr-x" ] || pc_fail "umask 022: mode is $mo, expected -rwxr-xr-x"
(umask 077; cp src dst2) >out 2>err </dev/null
modeof dst2 || pc_fail "cannot list dst2"
[ "$mo" = "-rwx------" ] || pc_fail "umask 077: mode is $mo, expected -rwx------"
pc_pass
