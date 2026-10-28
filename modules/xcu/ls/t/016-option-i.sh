# ls -i puts the file serial number before each name: equal for names of one file, different for different files.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.016
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.016
LC_ALL=POSIX; export LC_ALL
mkdir d || pc_fail "cannot create d"
: >d/a || pc_fail "cannot create d/a"
ln d/a d/b || pc_fail "cannot create hard link"
: >d/c || pc_fail "cannot create d/c"
pc_test
ls -i d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -i exited with status $rc"
na=; nb=; nc=
while read -r num name; do
	case $num in
	''|*[!0-9]*) pc_fail "serial number '$num' is not a number" ;;
	esac
	case $name in
	a) na=$num ;;
	b) nb=$num ;;
	c) nc=$num ;;
	esac
done <out
[ -n "$na" ] && [ -n "$nb" ] && [ -n "$nc" ] || pc_fail "an entry is missing from the output"
[ "$na" = "$nb" ] || pc_fail "the two names of one file have different serial numbers"
[ "$na" != "$nc" ] || pc_fail "different files have the same serial number"
pc_pass
