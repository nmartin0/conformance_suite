# rm -r removes a hierarchy deeper than a path name could be written in one piece.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rm.015
. "$PC_CORE/pc.sh"
pc_begin XCU.rm.015
base=$(pwd -P) || pc_fail "cannot determine the directory"
mkdir top || pc_fail "cannot create top"
cd top || pc_fail "cannot enter top"
i=0
while [ "$i" -lt 450 ]; do
	mkdir abcdefghijk || pc_fail "cannot create level $i"
	cd abcdefghijk || pc_fail "cannot enter level $i"
	i=$((i + 1))
done
printf x >leaf || pc_fail "cannot create leaf"
cd "$base" || pc_fail "cannot return"
pc_test
rm -r top >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "rm -r exited with status $rc"
[ ! -e top ] || pc_fail "top still exists"
pc_pass
