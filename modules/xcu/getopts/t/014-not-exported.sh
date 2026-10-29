# OPTIND and OPTARG are not exported.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getopts.014
. "$PC_CORE/pc.sh"
pc_begin XCU.getopts.014
PCMARK=1; export PCMARK
pc_test
set -- -c val
getopts c: pcopt
rc=$?
[ "$rc" -eq 0 ] || pc_fail "getopts exited $rc"
env >envout || pc_fail "env failed"
seen=
while IFS= read -r line; do
	case $line in
	OPTIND=*|OPTARG=*) pc_fail "exported: $line" ;;
	PCMARK=1) seen=1 ;;
	esac
done <envout
# env must have listed the environment at all, or the check proves nothing
[ -n "$seen" ] || pc_fail "env did not list the environment (no PCMARK)"
pc_pass
