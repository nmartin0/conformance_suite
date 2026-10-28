# The special parameter - expands to the current option letters, so a letter
# appears after the option is set with set and is gone after it is unset
# again.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.5-parameters.010
. "$PC_CORE/pc.sh"
pc_begin XCU.2.5-parameters.010
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'set -f\n'
	printf 'case $- in *f*) echo f1;; *) echo nof1;; esac\n'
	printf 'set +f\n'
	printf 'case $- in *f*) echo f2;; *) echo nof2;; esac\n'
	printf 'set -u\n'
	printf 'case $- in *u*) echo u;; *) echo nou;; esac\n'
	printf 'set +u\n'
	printf 'set -C\n'
	printf 'case $- in *C*) echo C;; *) echo noC;; esac\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'f1\nnof2\nu\nC\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
