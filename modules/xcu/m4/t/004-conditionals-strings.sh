# ifelse, incr, decr, len, index, substr, translit, shift
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.004
# ifelse chooses by string equality, handles the 4,5 and 6-or-more argument
# forms; incr, decr, len, index, substr, translit and shift give the
# specified strings.
. "$PC_CORE/pc.sh"
pc_begin XCU.m4.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >in <<'EOM'
ifelse(a,a,yes,no)|ifelse(a,b,yes,no)|ifelse(a,b,c)|ifelse(a,b,c,d,e,f,g)|incr(4)|decr(4)|len(hello)|index(hello,ll)|index(hello,z)|substr(hello,1,3)|substr(hello,3)|substr(hello,9)|translit(hello,el,ip)|translit(hello,el)|shift(a,b,c)
EOM
printf 'yes|no||g|5|3|5|2|-1|ell|lo||hippo|ho|b,c\n' >exp
pc_test
m4 in >out 2>err </dev/null || pc_fail "m4: nonzero status"
cmp out exp >/dev/null 2>&1 || pc_fail "output differs: $(cat out)"
pc_pass
