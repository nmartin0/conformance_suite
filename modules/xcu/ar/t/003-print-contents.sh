# ar -p writes member contents
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.003
# "ar -p a" writes the contents of all members in archive order; "ar -p a
# f2" writes only f2; with -v each member is preceded by a blank line, the
# name in angle brackets and another blank line (the name is the operand if
# operands are given, else the member name).
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\n' >f1; printf 'two\n' >f2; printf 'three\n' >f3
ar -rc a f1 f2 f3 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: cannot build the archive"
cat f1 f2 f3 >all.exp
printf '\n<f2>\n\n' >v2.exp; cat f2 >>v2.exp
{ printf '\n<f1>\n\n'; cat f1; printf '\n<f2>\n\n'; cat f2; printf '\n<f3>\n\n'; cat f3; } >vall.exp
pc_test
ar -p a >o1 2>err </dev/null || pc_fail "ar -p: nonzero status"
cmp -s o1 all.exp || pc_fail "ar -p a did not write all members in order"
ar -p a f2 >o2 2>err </dev/null || pc_fail "ar -p a f2: nonzero status"
cmp -s o2 f2 || pc_fail "ar -p a f2 did not write only f2"
ar -pv a f2 >o3 2>err </dev/null || pc_fail "ar -pv a f2: nonzero status"
cmp -s o3 v2.exp || pc_fail "ar -pv a f2 output differs from the specified header and contents"
ar -pv a >o4 2>err </dev/null || pc_fail "ar -pv a: nonzero status"
cmp -s o4 vall.exp || pc_fail "ar -pv a output differs from the specified headers and contents"
pc_pass
