# Minimum-value names have their fixed values.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getconf.007
. "$PC_CORE/pc.sh"
pc_begin XCU.getconf.007
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
chkv() {
	for how in shell exec; do
		if [ "$how" = shell ]; then
			got=$(getconf "$1" </dev/null 2>/dev/null)
		else
			got=$( (exec getconf "$1") </dev/null 2>/dev/null)
		fi
		[ "$got" = "$2" ] || pc_fail "$how: getconf $1 wrote '$got', wanted $2"
	done
}
pc_test
chkv _POSIX_ARG_MAX 4096
chkv _POSIX_NAME_MAX 14
chkv _POSIX_PATH_MAX 256
chkv _POSIX_PIPE_BUF 512
chkv _POSIX_OPEN_MAX 20
chkv _POSIX2_LINE_MAX 2048
pc_pass
