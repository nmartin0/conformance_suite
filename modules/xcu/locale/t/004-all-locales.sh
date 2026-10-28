# locale -a lists POSIX.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.locale.004
. "$PC_CORE/pc.sh"
pc_begin XCU.locale.004
LC_ALL=C; export LC_ALL
pc_test
locale -a >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "locale -a exited with status $rc"
p=no
while IFS= read -r l; do
	[ -n "$l" ] || pc_fail "locale -a wrote an empty line"
	[ "$l" = POSIX ] && p=yes
done <out
[ "$p" = yes ] || pc_fail "locale -a did not list POSIX"
pc_pass
