# In the long format mode, the owner execute position shows s for set-user-ID with execute and S without.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.010
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.010
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
}

LC_ALL=POSIX; export LC_ALL
mkdir d || pc_fail "cannot create d"
: >d/s1 || pc_fail "cannot create d/s1"
: >d/s2 || pc_fail "cannot create d/s2"
chmod 4755 d/s1 || pc_fail "cannot chmod d/s1"
chmod 4644 d/s2 || pc_fail "cannot chmod d/s2"
pc_test
modeof d/s1 || pc_fail "cannot list d/s1"
[ "$mo" = "-rwsr-xr-x" ] || pc_fail "mode 4755 is shown as $mo"
modeof d/s2 || pc_fail "cannot list d/s2"
[ "$mo" = "-rwSr--r--" ] || pc_fail "mode 4644 is shown as $mo"
pc_pass
