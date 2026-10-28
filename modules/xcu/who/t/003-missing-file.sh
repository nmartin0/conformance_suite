# who with a database file that does not exist.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.who.003
. "$PC_CORE/pc.sh"
pc_begin XCU.who.003
LC_ALL=C; export LC_ALL
f=pcwho_missing.$$
[ -e "$f" ] && pc_unresolved "setup: $f exists"
pc_test
who "$f" >out 2>err </dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "who of a nonexistent file exited with status 0"
pc_pass
