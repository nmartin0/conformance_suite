# Exit statuses for a utility that cannot be run.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nice.004
. "$PC_CORE/pc.sh"
pc_begin XCU.nice.004
nl='
'
# nv COMMAND...: the nice value of the process running COMMAND (a sh -c string
# using $$); sets nvv.
isint() {
	case $1 in
	''|-|*[!0-9-]*|?*-*) return 1 ;;
	esac
	return 0
}
printf 'exit 0\n' >noexec || pc_unresolved "setup: cannot create a file"
chmod 644 noexec || pc_unresolved "setup: chmod failed"
pc_test
nice pc_no_such_utility_zz </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 127 ] || pc_fail "nice of a missing utility exited $rc, wanted 127"
nice -n 1 pc_no_such_utility_zz </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 127 ] || pc_fail "nice -n 1 of a missing utility exited $rc, wanted 127"
nice ./noexec </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 126 ] || pc_fail "nice of a file that cannot be run exited $rc, wanted 126"
pc_pass
