# address offsets, errors and marks
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ed.018
# Offsets "+" and "-" with or without a number, a bare number or blank
# separated offsets add to an address; an intermediate value out of range is
# allowed but the final one is an error; a mark that was never set or whose
# line was deleted is an error.
. "$PC_CORE/pc.sh"
pc_begin XCU.ed.018
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
printf '3p\n-p\n+p\n++p\n--p\n$-1p\n1+2p\n1 3p\n2-5+4p\n2ka\n$kb\n'"'"'ap\n'"'"'bp\n' >s
printf 'three\ntwo\nthree\nfive\nthree\nfour\nthree\nfour\none\ntwo\nfive\n' >exp
printf '3-5p\n' >e1
printf '$+1p\n' >e2
printf '1 9p\n' >e3
printf "'cp\n" >e4
printf "2ka\n2d\n'ap\n" >e5
pc_test
ed -s f <s >o 2>err
cmp -s o exp || pc_fail "address offsets gave '$(tr '\n' ' ' <o)', expected '$(tr '\n' ' ' <exp)'"
for e in e1 e2 e3 e4 e5; do
	ed -s f <$e >o.$e 2>err
	[ "$(sed -n 1p o.$e)" = '?' ] || pc_fail "script $e ($(tr '\n' ' ' <$e)): no ? for the address error, got '$(cat o.$e)'"
done
pc_pass
