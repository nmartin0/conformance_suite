# ls -l of a directory starts with a total line and then one line per file with mode, link count, owner, group, size, a three-part date and the name.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.008
# MUTANT    ls.l_no_total
# MUTANT    ls.l_total_last
# MUTANT    ls.l_nlink_wrong
# MUTANT    ls.l_size_wrong
# MUTANT    ls.l_no_time
# MUTANT    ls.l_no_name
# MUTANT    ls.l_owner_group_swapped
# MUTANT    ls.l_month_number
# VARIANT   ls.acl_flag
# VARIANT   ls.dot_A
# VARIANT   ls.dot_a
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.008
LC_ALL=POSIX; export LC_ALL
mkdir d || pc_fail "cannot create d"
printf 'abcde' >d/f || pc_fail "cannot create d/f"
chmod 640 d/f || pc_fail "cannot chmod d/f"
# Give the file a group that is not the owner's, where the privilege allows
# it, so that the owner and group fields differ; failure is ignored.
chgrp "$(( $(id -g) + 54321 ))" d/f 2>/dev/null
pc_test
ls -l d >out0 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -l exited with status $rc"
# A listing may also hold dot and dot-dot (DESCRIPTION): drop their lines.
grep -v -e ' \.$' -e ' \.\.$' out0 >out
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
[ "$#" -ge 9 ] || pc_fail "only $# fields in the long format line, nine are required: $l2"
[ "$#" -eq 9 ] || pc_unresolved "unexpected number of fields ($#) in the long format line"
[ "$3" = "$(id -un)" ] || [ "$3" = "$(id -u)" ] || pc_fail "owner field is '$3', not the owner of the file"
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
