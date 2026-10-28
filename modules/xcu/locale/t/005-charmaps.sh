# locale -m lists charmap names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.locale.005
. "$PC_CORE/pc.sh"
pc_begin XCU.locale.005
LC_ALL=C; export LC_ALL
pc_test
locale -m >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "locale -m exited with status $rc"
n=0
while IFS= read -r l; do
	[ -n "$l" ] || pc_fail "locale -m wrote an empty line"
	n=$((n + 1))
done <out
[ "$n" -ge 1 ] || pc_fail "locale -m listed no charmap"
pc_pass
