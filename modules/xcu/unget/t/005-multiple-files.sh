# unget with several files and a directory
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.unget.005
. "$PC_CORE/pc.sh"
pc_begin XCU.unget.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
for u in get admin prs delta unget val; do command -v $u >/dev/null 2>&1 || pc_notinuse "$u is not available"; done
mkdir d || pc_unresolved "setup: mkdir"
for n in a b; do
	printf 'x\n' >$n || pc_unresolved "setup: write"
	admin -i$n d/s.$n </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin d/s.$n"
	rm -f $n
	get -e -s d/s.$n </dev/null >setup.out 2>&1 || pc_unresolved "setup: get -e d/s.$n"
done
rm -f a b
pc_test
unget d/s.a d/s.b </dev/null >out 2>err || pc_fail "unget of two files: nonzero status: $(cat err)"
printf '\nd/s.a:\n1.2\n\nd/s.b:\n1.2\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "output differs: $(od -c out | head -5)"
for n in a b; do
	get -e -s d/s.$n </dev/null >setup.out 2>&1 || pc_fail "get -e d/s.$n failed after unget"
done
rm -f a b
unget d </dev/null >out 2>err || pc_fail "unget of a directory: nonzero status: $(cat err)"
grep 's\.a:$' out >/dev/null || pc_fail "no heading for s.a with a directory: $(cat out)"
grep 's\.b:$' out >/dev/null || pc_fail "no heading for s.b with a directory: $(cat out)"
pc_pass
