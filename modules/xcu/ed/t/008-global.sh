# g and v
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ed.008
# g runs a command list on matching lines (default p), v on lines that do
# not match, and a multi-line list may carry an a command whose text ends
# the list.
. "$PC_CORE/pc.sh"
pc_begin XCU.ed.008
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
cat >s <<'EOS'
g/e/s/e/E/
,p
v/E/d
,p
g/rE/
g/onE/a\
appended
,p
Q
EOS
printf 'onE\ntwo\nthrEe\nfour\nfivE\nonE\nthrEe\nfivE\nthrEe\nonE\nappended\nthrEe\nfivE\n' >exp
pc_test
ed -s f <s >o 2>err
cmp -s o exp || pc_fail "global commands gave: $(cat o)"
pc_pass
