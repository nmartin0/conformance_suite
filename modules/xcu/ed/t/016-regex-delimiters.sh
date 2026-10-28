# delimiters and null RE
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ed.016
# Other delimiters can replace the slash (g%[%]%p, s-[0-9]--g), a delimiter
# in a bracket expression does not end the RE, // repeats the last RE, and a
# final delimiter may be omitted in an address.
. "$PC_CORE/pc.sh"
pc_begin XCU.ed.016
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'a%%b\nc9d\nq7\n' >r || pc_unresolved "setup: cannot write r"
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
cat >s <<'EOS'
g%[%]%p
2,3s-[0-9]--g
,p
/cd
Q
EOS
printf '/t/p\n//p\nQ\n' >s2
printf 'a%%b\na%%b\ncd\nq\ncd\n' >exp
printf 'two\nthree\n' >exp2
pc_test
ed -s r <s >o 2>err
cmp -s o exp || pc_fail "delimiter variants gave: $(cat o)"
ed -s f <s2 >o2 2>err
cmp -s o2 exp2 || pc_fail "null RE gave: $(cat o2)"
pc_pass
