# wrapscan option
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.019
# With wrapscan a search wraps past the end; with nowrapscan it fails.
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.019
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
cat >s.ex <<'EOS'
set ws
/one/p
q
EOS
cat >s2.ex <<'EOS'
set nows
/one/p
q
EOS
pc_test
ex -s f <s.ex >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "a wrapping search failed: status $rc"
[ "$(cat out)" = one ] || pc_fail "the wrapping search printed '$(cat out)'"
ex -s f <s2.ex >out 2>err
rc=$?
[ "$rc" -ne 0 ] || pc_fail "with nowrapscan the search past the end gave status 0"
[ -s out ] && pc_fail "with nowrapscan the search printed '$(cat out)'"
pc_pass
