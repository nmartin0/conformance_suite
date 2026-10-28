# Without -m the FIFO gets mode rw-rw-rw- reduced by the file creation mask.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mkfifo.002
. "$PC_CORE/pc.sh"
pc_begin XCU.mkfifo.002
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
}

pc_test
(umask 022; mkfifo f1) >out 2>err </dev/null
modeof f1 || pc_fail "cannot list f1"
[ "$mo" = "prw-r--r--" ] || pc_fail "umask 022: mode is $mo"
(umask 077; mkfifo f2) >out 2>err </dev/null
modeof f2 || pc_fail "cannot list f2"
[ "$mo" = "prw-------" ] || pc_fail "umask 077: mode is $mo"
pc_pass
