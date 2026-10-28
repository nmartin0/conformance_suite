# quit, write protection, x and wq
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.010
# q on a modified buffer fails, q! discards, w to an existing other file
# fails unless forced with !, -R blocks w but not w!, and x and wq write and
# exit.
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.010
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
printf 'orig\n' >other || pc_unresolved "setup: cannot write other"
cp f f.orig || pc_unresolved "setup: cannot copy"
cat >s.ex <<'EOS'
1d
q
EOS
cat >s2.ex <<'EOS'
1d
q!
EOS
cat >s3.ex <<'EOS'
1d
w other
q!
EOS
cat >s4.ex <<'EOS'
1d
w! other
q!
EOS
cat >s5.ex <<'EOS'
1d
w
q!
EOS
cat >s6.ex <<'EOS'
1d
w!
q
EOS
cat >s7.ex <<'EOS'
2d
x
EOS
cat >s8.ex <<'EOS'
1d
wq
EOS
pc_test
ex -s f <s.ex >out 2>err
rc=$?
[ "$rc" -ne 0 ] || pc_fail "q on a modified buffer gave status 0"
cmp f f.orig >/dev/null 2>&1 || pc_fail "q on a modified buffer changed the file"
ex -s f <s2.ex >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "q! gave status $rc"
cmp f f.orig >/dev/null 2>&1 || pc_fail "q! wrote the file"
ex -s f <s3.ex >out 2>err
rc=$?
[ "$rc" -ne 0 ] || pc_fail "w to an existing other file gave status 0"
[ "$(cat other)" = orig ] || pc_fail "w overwrote an existing other file"
ex -s f <s4.ex >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "w! other gave status $rc"
printf 'two\nthree\nfour\nfive\n' >exp
cmp other exp >/dev/null 2>&1 || pc_fail "w! did not write the other file: $(cat other)"
ex -s -R f <s5.ex >out 2>err
rc=$?
[ "$rc" -ne 0 ] || pc_fail "w with -R gave status 0"
cmp f f.orig >/dev/null 2>&1 || pc_fail "w with -R changed the file"
ex -s -R f <s6.ex >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "w! with -R gave status $rc"
cmp f exp >/dev/null 2>&1 || pc_fail "w! with -R did not write the file: $(cat f)"
cp f.orig f
ex -s f <s7.ex >out 2>err || pc_fail "x: nonzero status"
printf 'one\nthree\nfour\nfive\n' >exp
cmp f exp >/dev/null 2>&1 || pc_fail "x did not write the file: $(cat f)"
cp f.orig f
ex -s f <s8.ex >out 2>err || pc_fail "wq: nonzero status"
printf 'two\nthree\nfour\nfive\n' >exp
cmp f exp >/dev/null 2>&1 || pc_fail "wq did not write the file: $(cat f)"
pc_pass
