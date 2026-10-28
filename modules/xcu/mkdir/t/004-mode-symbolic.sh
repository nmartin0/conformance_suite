# mkdir -m with a symbolic mode applies it to an initial mode of rwxrwxrwx.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mkdir.004
. "$PC_CORE/pc.sh"
pc_begin XCU.mkdir.004
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
}

pc_test
umask 022
mkdir -m a-w d1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "mkdir -m a-w exited with status $rc"
modeof d1 || pc_fail "cannot list d1"
[ "$mo" = "dr-xr-xr-x" ] || pc_fail "-m a-w: mode is $mo"
mkdir -m o-rwx d2 >out 2>err </dev/null
modeof d2 || pc_fail "cannot list d2"
[ "$mo" = "drwxrwx---" ] || pc_fail "-m o-rwx: mode is $mo"
mkdir -m u=rwx,g=rx,o= d3 >out 2>err </dev/null
modeof d3 || pc_fail "cannot list d3"
[ "$mo" = "drwxr-x---" ] || pc_fail "-m u=rwx,g=rx,o=: mode is $mo"
mkdir -m go-w d4 >out 2>err </dev/null
modeof d4 || pc_fail "cannot list d4"
[ "$mo" = "drwxr-xr-x" ] || pc_fail "-m go-w: mode is $mo"
pc_pass
