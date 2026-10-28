# -u and -g (with -r) write one number and a newline.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.id.001
. "$PC_CORE/pc.sh"
pc_begin XCU.id.001
LC_ALL=C; export LC_ALL
nl='
'
pc_test
for o in "-u" "-g" "-ur" "-gr" "-u -r" "-g -r"; do
	id $o >out 2>/dev/null </dev/null || pc_fail "id $o failed"
	v=$(cat out; printf x)
	v=${v%x}
	case $v in
	*"$nl") ;;
	*) pc_fail "id $o output does not end in a newline" ;;
	esac
	v=${v%"$nl"}
	case $v in
	'' | *[!0-9]*) pc_fail "id $o wrote '$v', not one decimal number" ;;
	esac
done
pc_pass
