# alias with a name writes one name=value line.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.alias.002
. "$PC_CORE/pc.sh"
pc_begin XCU.alias.002
nl='
'
pc_test
alias pcal2='echo a b'
alias pcal2 >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "alias pcal2 exited $rc"
content=$(cat out; echo x)
content=${content%x}
line=${content%"$nl"}
[ "$content" != "$line" ] || pc_fail "output does not end with a newline"
case $line in
*"$nl"*) pc_fail "more than one line written" ;;
pcal2=*) ;;
*) pc_fail "line does not begin with pcal2=: $line" ;;
esac
pc_pass
