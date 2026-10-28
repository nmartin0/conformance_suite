# The permission X sets execute/search only for directories and for files that already have an execute bit.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chmod.006
. "$PC_CORE/pc.sh"
pc_begin XCU.chmod.006
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
}

: >f1 || pc_fail "cannot create f1"
: >f2 || pc_fail "cannot create f2"
mkdir d || pc_fail "cannot create d"
chmod 644 f1 || pc_fail "cannot chmod f1"
chmod 744 f2 || pc_fail "cannot chmod f2"
chmod 644 d || pc_fail "cannot chmod d"
pc_test
chmod a+X f1 f2 d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "chmod a+X exited with status $rc"
modeof f1 || pc_fail "cannot list f1"
[ "$mo" = "-rw-r--r--" ] || pc_fail "f1 (no execute bit) is $mo, expected -rw-r--r--"
modeof f2 || pc_fail "cannot list f2"
[ "$mo" = "-rwxr-xr-x" ] || pc_fail "f2 (owner execute) is $mo, expected -rwxr-xr-x"
modeof d || pc_fail "cannot list d"
[ "$mo" = "drwxr-xr-x" ] || pc_fail "directory is $mo, expected drwxr-xr-x"
pc_pass
