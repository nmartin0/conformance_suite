# Each option writes exactly one line ending in a newline.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uname.004
. "$PC_CORE/pc.sh"
pc_begin XCU.uname.004
LC_ALL=C; export LC_ALL
nl='
'
pc_test
for o in -m -n -r -s -v; do
	uname $o >out 2>/dev/null </dev/null || pc_fail "uname $o failed"
	v=$(cat out; printf x)
	v=${v%x}
	case $v in
	*"$nl") ;;
	*) pc_fail "uname $o output does not end in a newline" ;;
	esac
	v=${v%"$nl"}
	case $v in
	'') pc_fail "uname $o wrote an empty line" ;;
	*"$nl"*) pc_fail "uname $o wrote more than one line" ;;
	esac
done
pc_pass
