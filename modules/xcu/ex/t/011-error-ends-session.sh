# an error with a non-terminal input
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.011
# After an error with standard input not a terminal, ex does not write the
# file and exits with a non-zero status.
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.011
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
cp f f.orig || pc_unresolved "setup: cannot copy"
cat >s.ex <<'EOS'
1d
thisisnotacommand
w
q
EOS
cat >s2.ex <<'EOS'
1d
/nomatchatall/p
w
q
EOS
pc_test
ex -s f <s.ex >out 2>err
rc=$?
[ "$rc" -ne 0 ] || pc_fail "an invalid command did not give a non-zero status"
cmp f f.orig >/dev/null 2>&1 || pc_fail "ex wrote the file after an error"
ex -s f <s2.ex >out 2>err
rc=$?
[ "$rc" -ne 0 ] || pc_fail "a failed search did not give a non-zero status"
cmp f f.orig >/dev/null 2>&1 || pc_fail "ex wrote the file after a failed search"
pc_pass
