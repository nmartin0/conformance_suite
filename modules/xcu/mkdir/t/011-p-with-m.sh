# With -p and -m, only the last directory gets the -m mode; intermediates follow the -p rule.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mkdir.011
. "$PC_CORE/pc.sh"
pc_begin XCU.mkdir.011
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
}

pc_test
umask 022
mkdir -p -m 700 a/b >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "mkdir -p -m exited with status $rc"
modeof a || pc_fail "cannot list a"
[ "$mo" = "drwxr-xr-x" ] || pc_fail "intermediate a has mode $mo, expected drwxr-xr-x"
modeof a/b || pc_fail "cannot list a/b"
[ "$mo" = "drwx------" ] || pc_fail "a/b has mode $mo, expected drwx------"
pc_pass
