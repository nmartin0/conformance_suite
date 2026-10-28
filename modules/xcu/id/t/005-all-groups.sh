# id -G format and content.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.id.005
. "$PC_CORE/pc.sh"
pc_begin XCU.id.005
LC_ALL=C; export LC_ALL
nl='
'
pc_test
id -G >out 2>/dev/null </dev/null || pc_fail "id -G failed"
v=$(cat out; printf x)
v=${v%x}
case $v in
*"$nl") ;;
*) pc_fail "id -G output does not end in a newline" ;;
esac
line=${v%"$nl"}
case $line in
*"$nl"*) pc_fail "id -G wrote more than one line" ;;
'') pc_fail "id -G wrote an empty line" ;;
esac
set -- $line
[ "$*" = "$line" ] || pc_fail "'$line' is not separated by single spaces"
for w in "$@"; do
	case $w in
	'' | *[!0-9]*) pc_fail "'$w' is not a decimal group ID" ;;
	esac
done
n=0
for w in "$@"; do
	m=0
	for x in "$@"; do
		[ "$w" = "$x" ] && m=$((m + 1))
	done
	[ "$m" -eq 1 ] || pc_fail "group ID $w appears $m times"
done
eg=$(id -g)
rg=$(id -gr)
fe=no fr=no
for w in "$@"; do
	[ "$w" = "$eg" ] && fe=yes
	[ "$w" = "$rg" ] && fr=yes
done
[ "$fe" = yes ] || pc_fail "effective group ID $eg is not in '$line'"
[ "$fr" = yes ] || pc_fail "real group ID $rg is not in '$line'"
pc_pass
