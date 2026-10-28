# p, n, =, d and the current line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ed.003
# p prints lines, n prints each with its number and a tab, = prints a line
# number, and after d the current line is the line after the deleted ones
# (the new last line when the end was deleted, 0 when the buffer is empty).
. "$PC_CORE/pc.sh"
pc_begin XCU.ed.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
cat >s <<'EOS'
2,3p
.=
n
1n
$=
2,3d
.=
$d
.=
1,$d
.=
Q
EOS
printf 'two\nthree\n3\n3\tthree\n1\tone\n5\n2\n2\n0\n' >exp
pc_test
ed -s f <s >o 2>err
cmp -s o exp || pc_fail "output differs from the expected lines: $(cat o)"
pc_pass
