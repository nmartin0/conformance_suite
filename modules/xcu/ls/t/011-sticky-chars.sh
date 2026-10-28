# On a directory, the other-execute position shows t for the sticky bit with search permission and T without.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.011
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.011
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
}

LC_ALL=POSIX; export LC_ALL
mkdir d d/t1 d/t2 || pc_fail "cannot create directories"
chmod 1777 d/t1 || pc_fail "cannot chmod d/t1"
chmod 1776 d/t2 || pc_fail "cannot chmod d/t2"
pc_test
modeof d/t1 || pc_fail "cannot list d/t1"
[ "$mo" = "drwxrwxrwt" ] || pc_fail "mode 1777 is shown as $mo"
modeof d/t2 || pc_fail "cannot list d/t2"
[ "$mo" = "drwxrwxrwT" ] || pc_fail "mode 1776 is shown as $mo"
pc_pass
