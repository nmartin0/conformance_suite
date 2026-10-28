# Each resource option writes one limit value.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ulimit.003
. "$PC_CORE/pc.sh"
pc_begin XCU.ulimit.003
nl='
'
isval() {
	case $1 in
	unlimited) return 0 ;;
	''|*[!0-9]*) return 1 ;;
	esac
	return 0
}
pc_test
for opt in -c -d -n -s -v; do
	v=$(ulimit $opt 2>/dev/null)
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "ulimit $opt exited $rc"
	case $v in
	*"$nl"*) pc_fail "ulimit $opt wrote more than one line" ;;
	esac
	isval "$v" || pc_fail "ulimit $opt wrote '$v', neither digits nor unlimited"
done
pc_pass
