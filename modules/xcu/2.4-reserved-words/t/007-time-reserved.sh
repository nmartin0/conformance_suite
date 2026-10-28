# Where the reserved word time stands as the command name of a command that
# would run the time utility, the behaviour is that of the time utility:
# with -p the timings go to standard error in the POSIX format and the
# status is that of the timed command.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.4-reserved-words.007
. "$PC_CORE/pc.sh"
pc_begin XCU.2.4-reserved-words.007
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'time -p "$PCSH" -c \047exit 0\047\n'
	printf 'echo "rc=$?"\n'
	printf 'time -p "$PCSH" -c \047exit 3\047\n'
	printf 'echo "rc=$?"\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'rc=0\nrc=3\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
# the format allows one leading empty line and extra text after each number
nr=0; nu=0; ns=0
while IFS= read -r line; do
	set -- $line
	[ "$#" -ge 2 ] || continue
	case $2 in
	*[!0-9.]*|.|"") continue ;;
	esac
	case $1 in
	real) nr=$((nr+1)) ;;
	user) nu=$((nu+1)) ;;
	sys) ns=$((ns+1)) ;;
	esac
done <err
[ "$nr" -eq 2 ] && [ "$nu" -eq 2 ] && [ "$ns" -eq 2 ] || pc_fail "standard error did not hold two real/user/sys timing reports"
pc_pass
