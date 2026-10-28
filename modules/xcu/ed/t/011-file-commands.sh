# f, e, r and w and the remembered pathname
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ed.011
# f prints the remembered name and sets a new one; w with no name writes to
# it; e replaces the buffer; 0r reads a file at the top without changing the
# remembered name; w with a name writes those lines.
. "$PC_CORE/pc.sh"
pc_begin XCU.ed.011
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
cat >s <<'EOS'
f
f newname
w
e f
0r newname
$=
w out2
f
Q
EOS
printf 'f\nnewname\n10\nf\n' >exp
cat f f >exp2
pc_test
ed -s f <s >o 2>err
cmp -s o exp || pc_fail "f, e, r and w gave: $(cat o)"
cmp -s newname f || pc_fail "w with the new remembered name did not write the buffer"
cmp -s out2 exp2 || pc_fail "w out2 did not write the 10 lines"
pc_pass
