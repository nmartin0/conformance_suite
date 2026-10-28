# j, m, t, k and u
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ed.010
# j joins lines, m moves them after an address, t copies them after an
# address, u undoes the last change and restores the current line, and u
# after a g undoes all of its changes at once.
. "$PC_CORE/pc.sh"
pc_begin XCU.ed.010
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
cat >s <<'EOS'
1,2j
,p
1m$
,p
1t0
,p
u
.=
,p
g/o/s/o/0/
u
,p
Q
EOS
printf 'onetwo\nthree\nfour\nfive\nthree\nfour\nfive\nonetwo\nthree\nthree\nfour\nfive\nonetwo\n4\nthree\nfour\nfive\nonetwo\nthree\nfour\nfive\nonetwo\n' >exp
pc_test
ed -s f <s >o 2>err
cmp -s o exp || pc_fail "j, m, t and u gave: $(cat o)"
pc_pass
