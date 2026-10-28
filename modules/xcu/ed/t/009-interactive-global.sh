# G with & and null commands
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ed.009
# G writes each matching line and executes the next command read for it; &
# repeats the previous command.
. "$PC_CORE/pc.sh"
pc_begin XCU.ed.009
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
cat >s <<'EOS'
G/^t/
s/t/T/
&
,p
Q
EOS
printf 'two\nthree\none\nTwo\nThree\nfour\nfive\n' >exp
pc_test
ed -s f <s >o 2>err
cmp -s o exp || pc_fail "G gave: $(cat o)"
pc_pass
