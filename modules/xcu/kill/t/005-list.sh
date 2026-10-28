# kill -l writes the signal names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.kill.005
. "$PC_CORE/pc.sh"
pc_begin XCU.kill.005
nl='
'
pc_test
kill -l >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "kill -l exited $rc"
content=$(cat out; echo x)
content=${content%x}
[ -n "$content" ] || pc_fail "kill -l wrote nothing"
[ "${content%"$nl"}" != "$content" ] || pc_fail "the last name is not followed by a newline"
names=' '
for tok in $content; do
	case $tok in
	SIG*) pc_fail "name '$tok' has the SIG prefix" ;;
	*[!A-Z0-9+-]*) pc_fail "name '$tok' is not upper case letters and digits" ;;
	esac
	names="$names$tok "
done
for need in HUP INT QUIT ABRT KILL ALRM TERM USR1 USR2 CHLD CONT STOP TSTP; do
	case $names in
	*" $need "*) ;;
	*) pc_fail "$need is not in the list" ;;
	esac
done
pc_pass
