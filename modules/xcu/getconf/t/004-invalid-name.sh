# An unknown variable name is an error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getconf.004
. "$PC_CORE/pc.sh"
pc_begin XCU.getconf.004
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
getconf PC_NO_SUCH_VARIABLE_ZZ </dev/null >out 2>err
rc=$?
[ "$rc" -gt 0 ] || pc_fail "getconf of an unknown name exited zero"
[ ! -s out ] || pc_fail "getconf of an unknown name wrote to standard output: $(cat out)"
[ -s err ] || pc_fail "getconf of an unknown name wrote no diagnostic"
getconf PC_NO_SUCH_VARIABLE_ZZ . </dev/null >out 2>err
rc=$?
[ "$rc" -gt 0 ] || pc_fail "getconf of an unknown name with a path exited zero"
[ ! -s out ] || pc_fail "getconf of an unknown name with a path wrote to standard output"
[ -s err ] || pc_fail "getconf of an unknown name with a path wrote no diagnostic"
(exec getconf PC_NO_SUCH_VARIABLE_ZZ) </dev/null >out 2>err
rc=$?
[ "$rc" -gt 0 ] || pc_fail "exec getconf of an unknown name exited zero"
pc_pass
