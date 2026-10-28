# After an asynchronous AND-OR list the special parameter ! expands to the
# decimal process ID of that list, so that wait given it reports the list's
# exit status.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.5-parameters.009
. "$PC_CORE/pc.sh"
pc_begin XCU.2.5-parameters.009
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf '(exit 5) &\n'
	printf 'p=$!\n'
	printf 'case $p in \047\047|*[!0-9]*|0*) echo bad;; *) echo ok;; esac\n'
	printf 'wait $p\n'
	printf 'echo $?\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'ok\n5\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
