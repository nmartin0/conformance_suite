# When PATH names a directory holding executable files named alias, cd, command, getopts, hash, kill, read, type, ulimit, umask, unalias and wait, invoking each of those names as a simple command in the shell does not run the file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.7-intrinsic-utilities.001
. "$PC_CORE/pc.sh"
pc_begin XCU.1.7-intrinsic-utilities.001
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

mkdir bin || pc_unresolved "mkdir"
bindir=$(pwd)/bin
mark=$(pwd)/mark
for n in alias cd command getopts hash kill read type ulimit umask unalias wait; do
	{
		printf '#!%s\n' "$shp"
		printf '>>"%s"\n' "$mark"
		printf 'echo "$0" >>"%s"\n' "$mark"
	} >bin/$n
	chmod 755 bin/$n || pc_unresolved "chmod"
done
./bin/alias </dev/null >/dev/null 2>&1
[ -e mark ] || pc_unresolved "the override files cannot be executed"
rm -f mark
cat >script <<'EOS'
alias
cd .
command -v cd
getopts a x
hash
kill -l
read x
type cd
ulimit -f
umask
unalias -a
wait
EOS
pc_test
PATH=$bindir "$shp" ./script </dev/null >out 2>err &
pid=$!
i=0
while kill -0 "$pid" 2>/dev/null && [ "$i" -lt 30 ]; do
	sleep 1
	i=$((i + 1))
done
if kill -0 "$pid" 2>/dev/null; then
	kill -s KILL "$pid" 2>/dev/null
	wait "$pid" 2>/dev/null
	pc_fail "the shell did not finish"
fi
wait "$pid" 2>/dev/null
[ ! -e mark ] || pc_fail "a file from PATH ran in place of an intrinsic utility: $(tr '\n' ' ' <mark)"
pc_pass
