# -D, -U, -s, file operands and -
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.008
# -D defines to the value or to null, -U undefines (including built-ins),
# their order matters, options may follow operands, files are processed in
# order with - meaning standard input, and -s produces #line directives.
. "$PC_CORE/pc.sh"
pc_begin XCU.m4.008
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'x|y|z\n' >in
printf 'define(a,b)\n' >in2
printf 'one\n' >f1; printf 'two\n' >f2
printf 'from stdin\n' >sin
pc_test
m4 -D x=1 -D y in >out 2>err </dev/null || pc_fail "-D: nonzero status"
[ "$(cat out)" = '1||z' ] || pc_fail "-D x=1 -D y gave '$(cat out)'"
m4 -Dx=1 -Ux in >out 2>err </dev/null || pc_fail "-U: nonzero status"
[ "$(cat out)" = 'x|y|z' ] || pc_fail "-Dx=1 -Ux gave '$(cat out)'"
m4 -Ux -Dx=2 in >out 2>err </dev/null || pc_fail "-U then -D: nonzero status"
[ "$(cat out)" = '2|y|z' ] || pc_fail "-Ux -Dx=2 gave '$(cat out)'"
m4 -U define in2 >out 2>err </dev/null || pc_fail "-U define: nonzero status"
[ "$(cat out)" = 'define(a,b)' ] || pc_fail "-U define left it defined: '$(cat out)'"
m4 f1 - f2 >out 2>err <sin || pc_fail "file operands: nonzero status"
[ "$(tr '\n' ' ' <out)" = 'one from stdin two ' ] || pc_fail "operands gave '$(tr '\n' ' ' <out)'"
m4 f1 -Dx=1 in f2 >out 2>err </dev/null || pc_fail "interspersed option: nonzero status"
[ "$(tr '\n' ' ' <out)" = 'one 1|y|z two ' ] || pc_fail "interspersed -D gave '$(tr '\n' ' ' <out)'"
m4 -s f1 f2 >out 2>err </dev/null || pc_fail "-s: nonzero status"
grep '#line' out >/dev/null || pc_fail "-s wrote no #line directive"
grep -x one out >/dev/null && grep -x two out >/dev/null || pc_fail "-s lost the input text"
pc_pass
