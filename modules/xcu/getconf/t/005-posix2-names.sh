# POSIX2_ names equal the same names with an underscore in front.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getconf.005
. "$PC_CORE/pc.sh"
pc_begin XCU.getconf.005
nl='
'
# gv ARGS...: getconf ARGS must exit zero and write one line that is a
# number or the word undefined (judged the shell way and by exec). The
# values are left in gv_shell and gv_exec.
gv() {
	for how in shell exec; do
		if [ "$how" = shell ]; then
			getconf "$@" </dev/null >gv.out 2>/dev/null
		else
			(exec getconf "$@") </dev/null >gv.out 2>/dev/null
		fi
		rc=$?
		[ "$rc" -eq 0 ] || pc_fail "$how: getconf $* exited $rc"
		gv_n=0
		val=
		while IFS= read -r line; do
			gv_n=$((gv_n + 1))
			val=$line
		done <gv.out
		[ "$gv_n" -eq 1 ] || pc_fail "$how: getconf $* wrote $gv_n lines, wanted one"
		case $val in
		undefined) ;;
		*)
			num=${val#-}
			case $num in
			''|*[!0-9]*) pc_fail "$how: getconf $* wrote '$val', neither a number nor undefined" ;;
			esac
			;;
		esac
		eval "gv_$how=\$val"
	done
}
pc_test
for n in C_BIND C_DEV CHAR_TERM FORT_RUN LOCALEDEF SW_DEV UPE VERSION; do
	gv POSIX2_$n
	a_shell=$gv_shell a_exec=$gv_exec
	gv _POSIX2_$n
	[ "$a_shell" = "$gv_shell" ] || pc_fail "getconf POSIX2_$n gave '$a_shell' but getconf _POSIX2_$n gave '$gv_shell'"
	[ "$a_exec" = "$gv_exec" ] || pc_fail "exec getconf POSIX2_$n gave '$a_exec' but exec getconf _POSIX2_$n gave '$gv_exec'"
done
pc_pass
