# With PATH naming only an empty directory, type and command -v still find each of the twelve intrinsic utilities that do not depend on the User Portability Utilities option
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.7-intrinsic-utilities.002
. "$PC_CORE/pc.sh"
pc_begin XCU.1.7-intrinsic-utilities.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
shp=
case ${PC_SH:-sh} in
/*) shp=$PC_SH ;;
*)
	oldifs=$IFS
	IFS=:
	for d in $PATH; do
		[ -n "$d" ] || d=.
		if [ -x "$d/${PC_SH:-sh}" ]; then
			shp=$d/${PC_SH:-sh}
			break
		fi
	done
	IFS=$oldifs
	case $shp in /*) ;; *) shp=$(cd "$(dirname "$shp")" && pwd)/$(basename "$shp") ;; esac
	;;
esac
[ -x "$shp" ] || pc_unresolved "cannot find the shell by an absolute path"

mkdir empty || pc_unresolved "mkdir"
e=$(pwd)/empty
pc_test
for n in alias cd command getopts hash kill read type ulimit umask unalias wait; do
	r=$(PATH=$e "$shp" -c "type $n" 2>/dev/null </dev/null) || pc_fail "type $n failed with an empty PATH"
	case $r in *"$n"*) ;; *) pc_fail "type $n printed '$r'" ;; esac
	r=$(PATH=$e "$shp" -c "command -v $n" 2>/dev/null </dev/null) || pc_fail "command -v $n failed with an empty PATH"
	[ -n "$r" ] || pc_fail "command -v $n printed nothing"
done
pc_pass
