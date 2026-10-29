# user, ruser, group, rgroup fields.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ps.009
. "$PC_CORE/pc.sh"
pc_begin XCU.ps.009
LC_ALL=C; export LC_ALL
# chk FIELD NAME-OPTION ID-OPTION
chk() {
	ps -o "$1=" -p "$$" >out 2>err </dev/null || pc_fail "ps -o $1= failed"
	got=
	read -r got <out
	[ -n "$got" ] || pc_fail "ps -o $1= wrote nothing for process $$"
	nm=$(id "$2" 2>/dev/null)
	num=$(id "$3") || pc_fail "id $3 failed"
	[ -n "$num" ] || pc_fail "id $3 wrote nothing"
	if [ -n "$nm" ] && [ "$got" = "$nm" ]; then return 0; fi
	[ "$got" = "$num" ] && return 0
	pc_fail "$1 is '$got', expected '$nm' or '$num'"
}
pc_test
chk user -un -u
chk ruser -urn -ur
chk group -gn -g
chk rgroup -grn -gr
pc_pass
