# With set -C a > redirection does not replace an existing regular file (the
# command is not given the file and the file is unchanged), while >| does, a
# missing file is created, and a file that is not a regular file such as the
# null device is opened.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.7-redirection.004
. "$PC_CORE/pc.sh"
pc_begin XCU.2.7-redirection.004
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'echo old >f\n'
	printf 'set -C\n'
	printf 'echo new >f\n'
	printf 'cat f\n'
	printf 'echo forced >|f\n'
	printf 'cat f\n'
	printf 'echo n >fresh\n'
	printf 'cat fresh\n'
	printf 'echo x >/dev/null && echo devnull-ok\n'
	printf 'set +C\n'
	printf 'echo plain >f\n'
	printf 'cat f\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'old\nforced\nn\ndevnull-ok\nplain\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
