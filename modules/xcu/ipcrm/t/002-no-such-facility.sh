# ipcrm of a facility that does not exist.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ipcrm.002
. "$PC_CORE/pc.sh"
pc_begin XCU.ipcrm.002
LC_ALL=C; export LC_ALL
# Make sure that no existing facility has the identifier or the key used, so
# that nothing that belongs to anyone else is removed.
ipcs -a >chk 2>/dev/null </dev/null || pc_unresolved "setup: ipcs -a failed"
for w in $(cat chk); do
	case $w in
	2147483646 | 0x5a35167b | 0x5A35167B) pc_unresolved "setup: a facility with identifier or key '$w' exists" ;;
	esac
done
pc_test
for o in "-q 2147483646" "-s 2147483646" "-m 2147483646" "-Q 1513428603" "-S 1513428603" "-M 1513428603"; do
	ipcrm $o >out 2>err </dev/null
	rc=$?
	[ "$rc" -gt 0 ] || pc_fail "ipcrm $o exited with status 0"
done
pc_pass
