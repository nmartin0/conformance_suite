# A refused lowering of the nice value does not stop the utility.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nice.005
. "$PC_CORE/pc.sh"
pc_begin XCU.nice.005
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
pc_test
nice -n -5 "${PC_SH:-sh}" -c 'echo ran; exit 4' </dev/null >out 2>/dev/null
rc=$?
got=$(cat out)
[ "$got" = ran ] || pc_fail "the utility did not run after nice -n -5 (it wrote '$got')"
[ "$rc" -eq 4 ] || pc_fail "nice -n -5 exited $rc, wanted the utility's status 4"
pc_pass
