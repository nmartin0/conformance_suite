# -c and -r
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.012
# -c runs a command on the first file loaded and -r with no file lists
# recoverable files and exits normally.
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.012
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
cat >s.ex <<'EOS'
q
EOS
pc_test
ex -s -c '$d|w|q' f </dev/null >out 2>err || pc_fail "ex -c: nonzero status"
printf 'one\ntwo\nthree\nfour\n' >exp
cmp f exp >/dev/null 2>&1 || pc_fail "-c did not run the command on the loaded file: $(cat f)"
ex -r </dev/null >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ex -r with no file operand: exit status $rc"
pc_pass
