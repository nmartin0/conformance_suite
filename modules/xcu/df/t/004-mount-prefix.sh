# The mount point on the data line is a prefix of the absolute path of the file operand.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.df.004
. "$PC_CORE/pc.sh"
pc_begin XCU.df.004
here=$(pwd -P) || pc_fail "cannot determine the directory"
pc_test
df -P . >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "df -P exited with status $rc"
set -- $(sed -n 2p out)
[ "$#" -eq 6 ] || pc_unresolved "unexpected number of fields ($#) in the data line"
root=$6
if [ "$root" != / ]; then
	case $here in
	"$root"|"$root"/*) ;;
	*) pc_fail "mount point '$root' is not a prefix of '$here'" ;;
	esac
fi
pc_pass
