# address forms
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ed.004
# The forms 1, $, /RE/, ?RE?, +n, -, $-1, 1;/RE/, 'x marks, a comma alone
# and offsets select the expected lines; searches wrap around.
. "$PC_CORE/pc.sh"
pc_begin XCU.ed.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
cat >s <<'EOS'
1p
$p
/thr/p
?two?p
+2p
-p
$-1p
1;/f/p
2ka
4kb
'ap
'bp
,p
2+1p
$-2,$p
Q
EOS
printf 'one\nfive\nthree\ntwo\nfour\nthree\nfour\none\ntwo\nthree\nfour\ntwo\nfour\none\ntwo\nthree\nfour\nfive\nthree\nthree\nfour\nfive\n' >exp
pc_test
ed -s f <s >o 2>err
cmp -s o exp || pc_fail "addressed lines differ from the expected ones: $(cat o)"
pc_pass
