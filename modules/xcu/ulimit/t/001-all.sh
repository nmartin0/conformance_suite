# ulimit -a reports at least six limits.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ulimit.001
. "$PC_CORE/pc.sh"
pc_begin XCU.ulimit.001
pc_test
ulimit -a >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ulimit -a exited $rc"
n=0
while IFS= read -r line; do
	ok=
	for tok in $line; do
		case $tok in
		unlimited) ok=1 ;;
		''|*[!0-9]*) ;;
		*) ok=1 ;;
		esac
	done
	[ -z "$ok" ] || n=$((n + 1))
done <out
[ "$n" -ge 6 ] || pc_fail "ulimit -a wrote $n lines holding a limit value, wanted at least 6"
pc_pass
