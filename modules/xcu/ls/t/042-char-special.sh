# ls -l of a character special file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.042
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.042
LC_ALL=POSIX; export LC_ALL
[ -c /dev/null ] || pc_unresolved "/dev/null is not a character special file here"
pc_test
ls -l /dev/null >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -l /dev/null exited with status $rc"
[ "$(sed -n '$=' out)" = 1 ] || pc_fail "expected one line for the one operand: $(cat out)"
set -- $(cat out)
[ "$#" -ge 7 ] || pc_fail "fewer than 7 fields in the line: $(cat out)"
m10=${1%"${1#??????????}"}
case $m10 in
c?????????) ;;
*) pc_fail "the mode '$m10' does not start with c" ;;
esac
case $2 in
''|*[!0-9]*) pc_fail "the link count '$2' is not a number" ;;
esac
eval last=\${$#}
[ "$last" = /dev/null ] || pc_fail "the line ends with '$last', expected the pathname as given"
pc_pass
