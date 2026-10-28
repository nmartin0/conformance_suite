# XSI options -e, -u, -f, -l.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ps.008
. "$PC_CORE/pc.sh"
pc_begin XCU.ps.008
LC_ALL=C; export LC_ALL
eu=$(id -u) || pc_fail "id -u failed"
# inorder WORD...: the words of $l appear in this order (others may lie between).
inorder() {
	set -- "$@"
	rest=" $l "
	for w in "$@"; do
		case $rest in
		*" $w "*) rest=" ${rest#*" $w "}" ;;
		*) return 1 ;;
		esac
	done
	return 0
}
pc_test
ps -e -o pid= >out 2>err </dev/null || pc_fail "ps -e failed"
fa=no
while IFS= read -r l; do
	set -- $l
	[ "$1" = "$$" ] && fa=yes
done <out
[ "$fa" = yes ] || pc_fail "ps -e did not list $$"
ps -u "$eu" -o pid= >out 2>err </dev/null || pc_fail "ps -u $eu failed"
fa=no
while IFS= read -r l; do
	set -- $l
	[ "$1" = "$$" ] && fa=yes
done <out
[ "$fa" = yes ] || pc_fail "ps -u $eu did not list $$"
ps -f -p "$$" >out 2>err </dev/null || pc_fail "ps -f failed"
read -r l <out
inorder UID PID PPID C STIME TTY TIME CMD || pc_fail "ps -f headings are '$l'"
ps -l -p "$$" >out 2>err </dev/null || pc_fail "ps -l failed"
read -r l <out
inorder F S UID PID PPID C PRI NI ADDR SZ WCHAN TTY TIME CMD || pc_fail "ps -l headings are '$l'"
pc_pass
