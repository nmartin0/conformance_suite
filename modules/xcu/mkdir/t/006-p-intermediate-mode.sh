# The intermediate directories made by -p get u+wx plus everything the mask leaves, while the last one gets the plain default mode.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mkdir.006
. "$PC_CORE/pc.sh"
pc_begin XCU.mkdir.006
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
}

pc_test
(umask 0300; mkdir -p a/b) >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "mkdir -p exited with status $rc"
modeof a || pc_fail "cannot list a"
[ "$mo" = "drwxrwxrwx" ] || pc_fail "intermediate a has mode $mo, expected drwxrwxrwx"
modeof a/b || pc_fail "cannot list a/b"
[ "$mo" = "dr--rwxrwx" ] || pc_fail "last component a/b has mode $mo, expected dr--rwxrwx"
(umask 077; mkdir -p c/d) >out 2>err </dev/null
modeof c || pc_fail "cannot list c"
[ "$mo" = "drwx------" ] || pc_fail "umask 077: intermediate c has mode $mo"
pc_pass
