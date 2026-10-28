# POSIX2_ names equal the same names with an underscore in front.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getconf.005
. "$PC_CORE/pc.sh"
pc_begin XCU.getconf.005
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
for n in C_BIND C_DEV CHAR_TERM FORT_RUN LOCALEDEF SW_DEV UPE VERSION; do
	a=$(getconf POSIX2_$n </dev/null 2>/dev/null; echo "x$?")
	b=$(getconf _POSIX2_$n </dev/null 2>/dev/null; echo "x$?")
	[ "$a" = "$b" ] || pc_fail "getconf POSIX2_$n gave '$a' but getconf _POSIX2_$n gave '$b'"
done
pc_pass
