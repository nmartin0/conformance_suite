# OPTIND and OPTARG are not exported.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getopts.014
. "$PC_CORE/pc.sh"
pc_begin XCU.getopts.014
pc_test
set -- -c val
getopts c: pcopt
rc=$?
[ "$rc" -eq 0 ] || pc_fail "getopts exited $rc"
env >envout || pc_fail "env failed"
while IFS= read -r line; do
	case $line in
	OPTIND=*|OPTARG=*) pc_fail "exported: $line" ;;
	esac
done <envout
pc_pass
