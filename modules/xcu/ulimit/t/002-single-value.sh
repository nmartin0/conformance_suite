# ulimit -f and its variants write one limit value.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ulimit.002
. "$PC_CORE/pc.sh"
pc_begin XCU.ulimit.002
nl='
'
isval() {
	case $1 in
	unlimited) return 0 ;;
	''|*[!0-9]*) return 1 ;;
	esac
	return 0
}
pc_test
a=$(ulimit -f 2>/dev/null) || pc_fail "ulimit -f failed"
b=$(ulimit -S -f 2>/dev/null) || pc_fail "ulimit -S -f failed"
c=$(ulimit -H -f 2>/dev/null) || pc_fail "ulimit -H -f failed"
d=$(ulimit 2>/dev/null) || pc_fail "ulimit with no option failed"
for v in "$a" "$b" "$c" "$d"; do
	case $v in
	*"$nl"*) pc_fail "more than one line written: '$v'" ;;
	esac
	isval "$v" || pc_fail "'$v' is neither digits nor unlimited"
done
[ "$d" = "$a" ] || pc_fail "ulimit gives '$d' but ulimit -f gives '$a'"
[ "$a" = "$b" ] || pc_fail "ulimit -f gives '$a' but ulimit -S -f gives '$b' (the soft limit is the default)"
pc_pass
