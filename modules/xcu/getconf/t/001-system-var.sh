# sysconf variables give one integer line or undefined.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getconf.001
. "$PC_CORE/pc.sh"
pc_begin XCU.getconf.001
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
		case ${val#-} in
		undefined) ;;
		''|*[!0-9]*) pc_fail "$how: getconf $* wrote '$val', neither digits nor undefined" ;;
		esac
	done
}
pc_test
for v in _POSIX_VERSION ARG_MAX CHILD_MAX OPEN_MAX PAGESIZE LINE_MAX; do
	gv $v
done
pc_pass
