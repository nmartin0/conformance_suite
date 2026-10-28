# Without -m the directory gets mode rwxrwxrwx reduced by the file creation mask.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mkdir.002
. "$PC_CORE/pc.sh"
pc_begin XCU.mkdir.002
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
}

pc_test
(umask 022; mkdir d1) >out 2>err </dev/null
modeof d1 || pc_fail "cannot list d1"
[ "$mo" = "drwxr-xr-x" ] || pc_fail "umask 022: mode is $mo"
(umask 077; mkdir d2) >out 2>err </dev/null
modeof d2 || pc_fail "cannot list d2"
[ "$mo" = "drwx------" ] || pc_fail "umask 077: mode is $mo"
(umask 000; mkdir d3) >out 2>err </dev/null
modeof d3 || pc_fail "cannot list d3"
[ "$mo" = "drwxrwxrwx" ] || pc_fail "umask 000: mode is $mo"
pc_pass
