# mkfifo -m with a symbolic mode applies it to an initial mode of rw-rw-rw-.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mkfifo.004
. "$PC_CORE/pc.sh"
pc_begin XCU.mkfifo.004
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
}

pc_test
umask 022
mkfifo -m a-w f1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "mkfifo -m a-w exited with status $rc"
modeof f1 || pc_fail "cannot list f1"
[ "$mo" = "pr--r--r--" ] || pc_fail "-m a-w: mode is $mo"
mkfifo -m go-rw f2 >out 2>err </dev/null
modeof f2 || pc_fail "cannot list f2"
[ "$mo" = "prw-------" ] || pc_fail "-m go-rw: mode is $mo"
mkfifo -m o-w f3 >out 2>err </dev/null
modeof f3 || pc_fail "cannot list f3"
[ "$mo" = "prw-rw-r--" ] || pc_fail "-m o-w: mode is $mo"
pc_pass
