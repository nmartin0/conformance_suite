# getconf PATH writes a PATH that finds sh and ls.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getconf.003
. "$PC_CORE/pc.sh"
pc_begin XCU.getconf.003
nl='
'
# gv ARGS...: getconf ARGS must exit zero and write one line that is
# digits or the word undefined (judged the shell way and by exec).
gv() {
	for how in shell exec; do
		if [ "$how" = shell ]; then
			getconf "$@" </dev/null >gv.out 2>/dev/null
		else
			(exec getconf "$@") </dev/null >gv.out 2>/dev/null
		fi
		rc=$?
		[ "$rc" -eq 0 ] || pc_fail "$how: getconf $* exited $rc"
		n=0
		val=
		while IFS= read -r line; do
			n=$((n + 1))
			val=$line
		done <gv.out
		[ "$n" -eq 1 ] || pc_fail "$how: getconf $* wrote $n lines, wanted one"
		case $val in
		undefined) ;;
		''|*[!0-9]*) pc_fail "$how: getconf $* wrote '$val', neither digits nor undefined" ;;
		esac
	done
}
pc_test
getconf PATH </dev/null >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "getconf PATH exited $rc"
n=0
val=
while IFS= read -r line; do
	n=$((n + 1))
	val=$line
done <out
[ "$n" -eq 1 ] || pc_fail "getconf PATH wrote $n lines, wanted one"
[ -n "$val" ] || pc_fail "getconf PATH wrote an empty line"
(
	PATH=$val
	command -v sh >/dev/null 2>&1 || exit 3
	command -v ls >/dev/null 2>&1 || exit 4
	exit 0
)
rc=$?
case $rc in
0) ;;
3) pc_fail "sh is not found with PATH=$val" ;;
4) pc_fail "ls is not found with PATH=$val" ;;
*) pc_fail "setup: the PATH check ended with status $rc" ;;
esac
pc_pass
