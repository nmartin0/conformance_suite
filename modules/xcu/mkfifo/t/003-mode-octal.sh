# mkfifo -m with an octal mode sets exactly that mode, whatever the file creation mask.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mkfifo.003
. "$PC_CORE/pc.sh"
pc_begin XCU.mkfifo.003
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
}

pc_test
(umask 077; mkfifo -m 664 f1) >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "mkfifo -m 664 exited with status $rc"
modeof f1 || pc_fail "cannot list f1"
[ "$mo" = "prw-rw-r--" ] || pc_fail "-m 664 under umask 077: mode is $mo"
pc_pass
