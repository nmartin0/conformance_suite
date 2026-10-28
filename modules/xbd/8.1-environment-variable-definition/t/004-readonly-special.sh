# Marking PWD, OLDPWD, OPTIND, OPTARG or LINENO readonly is rejected, or
# succeeds with the shell still modifying the variable, or succeeds with
# the built-in that would modify it failing.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.8.1-environment-variable-definition.004
#
. "$PC_CORE/pc.sh"
pc_begin XBD.8.1-environment-variable-definition.004
sh_=${PC_SH:-sh}
mkdir d1 d2 || pc_unresolved "cannot make directories"
d1=$(cd d1 && pwd -P) || pc_unresolved "cannot find d1"
d2=$(cd d2 && pwd -P) || pc_unresolved "cannot find d2"
pc_test
# rejected: the shell ended or readonly failed before reaching the marker
ro() {
	"$sh_" -c "readonly $1; echo reached" >ro.out 2>/dev/null </dev/null
	ro_rc=$?
	ro_reached=
	ro_line=
	read ro_line <ro.out
	[ "$ro_line" = reached ] && ro_reached=1
}
for v in PWD OLDPWD OPTIND OPTARG LINENO; do
	ro "$v"
	if [ -z "$ro_reached" ]; then
		[ "$ro_rc" -ne 0 ] || pc_fail "readonly $v ended the shell with status 0 without completing"
		continue
	fi
	# accepted: the shell, or the built-in that modifies it, must still cope
	case $v in
	PWD)
		out=$("$sh_" -c 'readonly PWD; cd "$1"; echo "$?:$PWD"' sh "$d2" 2>/dev/null </dev/null)
		case $out in
		0:"$d2") ;;
		0:*) pc_fail "readonly PWD accepted, cd succeeded but PWD is not the new directory: $out" ;;
		esac
		;;
	OLDPWD)
		out=$("$sh_" -c 'cd "$1" || exit 90; readonly OLDPWD; cd "$2"; echo "$?:$OLDPWD"' sh "$d1" "$d2" 2>/dev/null </dev/null)
		case $out in
		0:"$d1") ;;
		0:*) pc_fail "readonly OLDPWD accepted, cd succeeded but OLDPWD is not the previous directory: $out" ;;
		esac
		;;
	OPTIND)
		out=$("$sh_" -c 'readonly OPTIND; getopts a x -a; echo "$?:$OPTIND"' sh 2>/dev/null </dev/null)
		case $out in
		0:2) ;;
		0:*) pc_fail "readonly OPTIND accepted, getopts succeeded but OPTIND was not updated: $out" ;;
		esac
		;;
	OPTARG)
		out=$("$sh_" -c 'readonly OPTARG; getopts a: x -a val; echo "$?:$OPTARG"' sh 2>/dev/null </dev/null)
		case $out in
		0:val) ;;
		0:*) pc_fail "readonly OPTARG accepted, getopts succeeded but OPTARG is not the argument: $out" ;;
		esac
		;;
	LINENO)
		out=$("$sh_" -c 'readonly LINENO
a=$LINENO
b=$LINENO
echo "$a:$b"' sh 2>/dev/null </dev/null)
		case $out in
		*:*)
			[ "${out%%:*}" != "${out#*:}" ] || pc_fail "readonly LINENO accepted but LINENO does not change between lines: $out"
			;;
		esac
		;;
	esac
done
pc_pass
