# ls -s puts a total line first and the block count before each name.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.017
# MUTANT    ls.s_ignored
# MUTANT    ls.s_no_total
# MUTANT    ls.s_no_count
# MUTANT    ls.s_after_name
# MUTANT    ls.s_total_last
# VARIANT   ls.s_units
# VARIANT   ls.dot_A
# VARIANT   ls.dot_a
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.017
LC_ALL=POSIX; export LC_ALL
mkdir d || pc_fail "cannot create d"
printf 'abc' >d/a || pc_fail "cannot create d/a"
: >d/b || pc_fail "cannot create d/b"
pc_test
ls -s d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -s exited with status $rc"
l1=$(sed -n 1p out)
case $l1 in
"total "*) t=${l1#total } ;;
*) pc_fail "first line is not a total line" ;;
esac
case $t in
''|*[!0-9]*) pc_fail "the total '$t' is not a number" ;;
esac
sed -n '2,$p' out >rest || pc_fail "cannot post-process the output"
seen=
while read -r num name; do
	case $name in
	.|..) continue ;;
	esac
	case $num in
	''|*[!0-9]*) pc_fail "block count '$num' is not a number" ;;
	esac
	seen=$seen$name
done <rest
[ "$seen" = ab ] || pc_fail "entries are '$seen', expected a and b"
pc_pass
