# ! command, r !cmd, w !cmd and E !cmd
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ed.012
# "!cmd" runs the command and, unless -s, writes "!" after it; a % in it
# becomes the remembered pathname and the changed line is written first; r
# !cmd appends the command's output; w !cmd feeds the lines to the command;
# E !cmd replaces the buffer.
. "$PC_CORE/pc.sh"
pc_begin XCU.ed.012
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
cat >s <<'EOS'
!echo shell-out
r !echo from-cmd
$p
w !cat > wcmd.out
E !echo only-this
,p
Q
EOS
printf '!echo hi\n!echo %%\nq\n' >s2
printf 'shell-out\nfrom-cmd\nonly-this\n' >exp
printf '24\nhi\n!\necho f\nf\n!\n' >exp2
printf '24\nhi\n!\n!echo f\nf\n!\n' >exp2b
cat f >exp3; printf 'from-cmd\n' >>exp3
pc_test
ed -s f <s >o 2>err
cmp -s o exp || pc_fail "shell commands under -s gave: $(cat o)"
cmp -s wcmd.out exp3 || pc_fail "w !cmd did not feed the lines to the command"
ed f <s2 >o2 2>err
cmp -s o2 exp2 || cmp -s o2 exp2b || pc_fail "shell escape without -s gave: $(cat o2)"
pc_pass
