# On a directory, a+t or t alone sets the sticky bit, octal 1777 sets it, and an octal mode without it clears it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chmod.008
. "$PC_CORE/pc.sh"
pc_begin XCU.chmod.008
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
}

mkdir d1 d2 d3 || pc_fail "cannot create directories"
chmod 777 d1 d2 d3 || pc_fail "cannot chmod"
pc_test
chmod a+t d1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "chmod a+t exited with status $rc"
modeof d1 || pc_fail "cannot list d1"
[ "$mo" = "drwxrwxrwt" ] || pc_fail "a+t: mode is $mo"
chmod +t d2 >out 2>err </dev/null
modeof d2 || pc_fail "cannot list d2"
[ "$mo" = "drwxrwxrwt" ] || pc_fail "+t: mode is $mo"
chmod 1777 d3 >out 2>err </dev/null
modeof d3 || pc_fail "cannot list d3"
[ "$mo" = "drwxrwxrwt" ] || pc_fail "1777: mode is $mo"
chmod 777 d3 >out 2>err </dev/null
modeof d3 || pc_fail "cannot list d3"
[ "$mo" = "drwxrwxrwx" ] || pc_fail "777 after 1777: mode is $mo"
pc_pass
