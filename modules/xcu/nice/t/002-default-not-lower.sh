# By default the nice value is not lowered.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nice.002
. "$PC_CORE/pc.sh"
pc_begin XCU.nice.002
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
mine=$(ps -o nice= -p $$ 2>/dev/null)
mine=$(echo $mine)
isint "$mine" || pc_unresolved "setup: ps -o nice= -p gave '$mine'"
pc_test
nice "${PC_SH:-sh}" -c 'ps -o nice= -p $$' </dev/null >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "nice exited $rc"
child=$(cat out)
child=$(echo $child)
isint "$child" || pc_unresolved "ps gave '$child' for the process run by nice"
[ "$child" -ge "$mine" ] || pc_fail "the utility has nice value $child, lower than the caller's $mine"
pc_pass
