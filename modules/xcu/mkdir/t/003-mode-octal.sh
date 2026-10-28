# mkdir -m with an octal mode sets exactly that mode, whatever the file creation mask.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mkdir.003
. "$PC_CORE/pc.sh"
pc_begin XCU.mkdir.003
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
}

pc_test
(umask 077; mkdir -m 755 d1) >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "mkdir -m 755 exited with status $rc"
modeof d1 || pc_fail "cannot list d1"
[ "$mo" = "drwxr-xr-x" ] || pc_fail "-m 755 under umask 077: mode is $mo"
(umask 022; mkdir -m 700 d2) >out 2>err </dev/null
modeof d2 || pc_fail "cannot list d2"
[ "$mo" = "drwx------" ] || pc_fail "-m 700: mode is $mo"
(umask 022; mkdir -m 0777 d3) >out 2>err </dev/null
modeof d3 || pc_fail "cannot list d3"
[ "$mo" = "drwxrwxrwx" ] || pc_fail "-m 0777 under umask 022: mode is $mo"
pc_pass
