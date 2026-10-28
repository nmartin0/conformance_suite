# -s suppresses EXINIT and .exrc
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.013
# With -s the EXINIT variable and the .exrc file are not used.
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.013
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
mkdir home || pc_unresolved "setup: cannot make a directory"
printf '1d\n' >home/.exrc || pc_unresolved "setup: cannot write .exrc"
cp f f.orig || pc_unresolved "setup: cannot copy"
cat >s.ex <<'EOS'
w
q
EOS
pc_test
EXINIT='1d' HOME=$PWD/home ex -s f <s.ex >out 2>err || pc_fail "ex -s: nonzero status"
cmp f f.orig >/dev/null 2>&1 || pc_fail "EXINIT or .exrc was used although -s was given: $(cat f)"
pc_pass
