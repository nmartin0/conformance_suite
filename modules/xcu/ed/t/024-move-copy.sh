# m and t destinations
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ed.024
# m moves lines after the destination and makes the last moved line current;
# a destination inside the moved range is an error; t copies to 0 or to the
# end and makes the last copy current.
. "$PC_CORE/pc.sh"
pc_begin XCU.ed.024
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
# the buffer is changed, so each script ends with Q (end of input acts as q, which warns on a changed buffer);
# p moves the current line to the last line written, so .= comes before ,p
printf '1,2m4\n.=\n,p\n2,4m3\nQ\n' >s1
printf '4\nthree\nfour\none\ntwo\nfive\n?\n' >exp1
printf '2t0\n.=\n,p\n1,2t$\n.=\nQ\n' >s2
printf '1\ntwo\none\ntwo\nthree\nfour\nfive\n8\n' >exp2
pc_test
ed -s f <s1 >o1 2>err
cmp -s o1 exp1 || pc_fail "m gave '$(tr '\n' ' ' <o1)', expected '$(tr '\n' ' ' <exp1)'"
ed -s f <s2 >o2 2>err
cmp -s o2 exp2 || pc_fail "t gave '$(tr '\n' ' ' <o2)', expected '$(tr '\n' ' ' <exp2)'"
pc_pass
