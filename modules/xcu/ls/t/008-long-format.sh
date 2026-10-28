# ls -l of a directory starts with a total line and then one line per file with mode, link count, owner, group, size, a three-part date and the name.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.008
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.008
LC_ALL=POSIX; export LC_ALL
mkdir d || pc_fail "cannot create d"
printf 'abcde' >d/f || pc_fail "cannot create d/f"
chmod 640 d/f || pc_fail "cannot chmod d/f"
pc_test
ls -l d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -l exited with status $rc"
n=$(sed -n '$=' out)
[ "$n" = 2 ] || pc_fail "expected 2 lines, found $n"
l1=$(sed -n 1p out)
l2=$(sed -n 2p out)
case $l1 in
"total "*) t=${l1#total } ;;
*) pc_fail "first line is not a total line" ;;
esac
case $t in
''|*[!0-9]*) pc_fail "the total '$t' is not a number" ;;
esac
set -- $l2
[ "$#" -eq 9 ] || pc_unresolved "unexpected number of fields ($#) in the long format line"
m10=${1%"${1#??????????}"}
[ "$m10" = "-rw-r-----" ] || pc_fail "mode field is $m10"
[ "$2" = 1 ] || pc_fail "link count is $2"
[ "$5" = 5 ] || pc_fail "size is $5"
case $6 in
[A-Z][a-z][a-z]) ;;
*) pc_fail "month field is '$6'" ;;
esac
case $7 in
''|*[!0-9]*) pc_fail "day field is '$7'" ;;
esac
case $8 in
[0-9][0-9]:[0-9][0-9]|[0-9][0-9][0-9][0-9]) ;;
*) pc_fail "time or year field is '$8'" ;;
esac
[ "$9" = f ] || pc_fail "name field is '$9'"
pc_pass
