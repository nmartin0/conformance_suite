# pwd writes one line with an absolute pathname of the current directory that has no dot or dot-dot components and no needless slashes, both as the shell runs it and by a direct exec.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pwd.001
. "$PC_CORE/pc.sh"
pc_begin XCU.pwd.001
chkpath() {
	case $1 in
	/*) ;;
	*) pc_fail "$2: not an absolute pathname: $1" ;;
	esac
	case $1 in
	*/./*|*/../*|*/.|*/..) pc_fail "$2: contains dot or dot-dot: $1" ;;
	esac
	cp_p=$1
	case $cp_p in
	//[!/]*) cp_p=${cp_p#/} ;;
	esac
	case ${cp_p#/} in
	*//*) pc_fail "$2: contains doubled slashes: $1" ;;
	esac
	[ "$1" -ef . ] || pc_fail "$2: does not name the current directory: $1"
}

mkdir real || pc_fail "cannot create real"
cd real || pc_fail "cannot enter real"
pc_test
out=$(pwd)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "pwd exited with status $rc"
chkpath "$out" "pwd"
out=$( (exec pwd) )
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exec pwd exited with status $rc"
chkpath "$out" "exec pwd"
(exec pwd) >out2 2>err </dev/null
n=$(sed -n '$=' out2)
[ "$n" = 1 ] || pc_fail "pwd wrote $n lines"
pc_pass
