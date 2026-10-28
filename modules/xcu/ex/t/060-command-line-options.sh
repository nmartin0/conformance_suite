# command-line options
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.060
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.060
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'a\nb\nc\nd\ne\n' >f0
printf 'q!\n' >qq
printf '99d\nq\n' >bad
pc_test
cp f0 f
ex -sR -- f <qq >o1 2>e1 || pc_fail "ex -sR -- f: exit status $?"
cp f0 f
ex -sc '1d|w|q' f <qq >o2 2>e2 || pc_fail "ex -sc: exit status $?"
printf 'b\nc\nd\ne\n' >exp
cmp -s f exp || pc_fail "options grouped in one argument were not understood: $(tr '\n' '|' <f)"
cp f0 f
ex -s -c '.=' f <qq >o3 2>e3 || pc_fail "ex -c: exit status $?"
[ "$(cat o3)" = 5 ] || pc_fail "the current line for -c is '$(cat o3)', wanted the last line 5"
cp f0 f
if ex -s -c 's/a/b/' -c 's/b/c/' -c w f <qq >o4 2>e4; then
	[ "$(sed -n 1p f)" = c ] || pc_fail "a second -c was not run after the first: '$(sed -n 1p f)'"
else
	pc_notinuse "more than one -c is not supported"
fi
cp f0 f
ex -s f <bad >o5 2>e5
r=$?
[ "$r" -ne 0 ] || pc_fail "99d on five lines was accepted"
[ -s o5 ] || [ -s e5 ] || pc_fail "no message was written for the error"
pc_pass
