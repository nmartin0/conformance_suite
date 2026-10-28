# ${p%w} removes the smallest suffix matching the pattern w and ${p%%w} the
# largest; the pattern is a shell pattern, and a value with no matching
# suffix, or an empty pattern, leaves p unchanged.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.019
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.019
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'x=file.c.o\n'
	printf 'echo ${x%%.*} ${x%%%%.*} ${x%%o}\n'
	printf 'x=posix/src/std\n'
	printf 'echo ${x%%%%/*} ${x%%/*}\n'
	printf 'x=abc\n'
	printf 'echo ${x%%b*} ${x%%?} ${x%%[a-b]} ${x%%zz} ${x%%}\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'file.c file file.c.\nposix posix/src\na ab abc abc abc\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
