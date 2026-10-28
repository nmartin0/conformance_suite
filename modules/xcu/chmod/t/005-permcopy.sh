# The permission letters u, g and o after an operator copy the current permissions of that class.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chmod.005
. "$PC_CORE/pc.sh"
pc_begin XCU.chmod.005
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
}

: >f1 || pc_fail "cannot create f1"
: >f2 || pc_fail "cannot create f2"
: >f3 || pc_fail "cannot create f3"
chmod 750 f1 || pc_fail "cannot chmod f1"
chmod 640 f2 || pc_fail "cannot chmod f2"
chmod 700 f3 || pc_fail "cannot chmod f3"
pc_test
chmod g=u f1 >out 2>err </dev/null
modeof f1 || pc_fail "cannot list f1"
[ "$mo" = "-rwxrwx---" ] || pc_fail "g=u: mode is $mo, expected -rwxrwx---"
chmod o=g f2 >out 2>err </dev/null
modeof f2 || pc_fail "cannot list f2"
[ "$mo" = "-rw-r--r--" ] || pc_fail "o=g: mode is $mo, expected -rw-r--r--"
chmod go=u f3 >out 2>err </dev/null
modeof f3 || pc_fail "cannot list f3"
[ "$mo" = "-rwxrwxrwx" ] || pc_fail "go=u: mode is $mo, expected -rwxrwxrwx"
pc_pass
