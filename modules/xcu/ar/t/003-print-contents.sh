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
# Archive order differs from name order; members of odd length, with no
# final newline, empty and larger than a buffer are included.
printf 'three\n' >f3; printf 'one\n' >f1; printf 'two\n' >f2
printf 'odd' >f4; : >f5
i=0; while [ "$i" -lt 1500 ]; do printf 'line %s of the large member\n' "$i"; i=$((i+1)); done >f6
printf '\nstarts with a newline' >f7
ar -rc a f3 f1 f2 f4 f5 f6 f7 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: cannot build the archive"
cat f3 f1 f2 f4 f5 f6 f7 >all.exp
printf '\n<f2>\n\n' >v2.exp; cat f2 >>v2.exp
: >vall.exp
for f in f3 f1 f2 f4 f5 f6 f7; do { printf '\n<%s>\n\n' "$f"; cat "$f"; } >>vall.exp; done
printf 'one\nthree\n' >lines.exp
pc_test
ar -p a >o1 2>err </dev/null || pc_fail "ar -p: nonzero status"
[ -f o1 ] || pc_fail "no output file"
cmp -s o1 all.exp || pc_fail "ar -p a did not write all members in order"
ar -p a f2 >o2 2>err </dev/null || pc_fail "ar -p a f2: nonzero status"
[ -f o2 ] || pc_fail "no output file"
cmp -s o2 f2 || pc_fail "ar -p a f2 did not write only f2"
ar -p a f6 >o5 2>err </dev/null || pc_fail "ar -p a f6: nonzero status"
[ -f o5 ] || pc_fail "no output file"
cmp -s o5 f6 || pc_fail "ar -p a f6 did not write the large member whole"
ar -p a f4 >o5 2>err </dev/null || pc_fail "ar -p a f4: nonzero status"
[ -f o5 ] || pc_fail "no output file"
cmp -s o5 f4 || pc_fail "ar -p a f4 did not write the member of odd length exactly"
ar -p a f1 f3 >o6 2>err </dev/null || pc_fail "ar -p a f1 f3: nonzero status"
[ -f o6 ] || pc_fail "no output file"
# The order of the contents for several operands is not fixed.
sort o6 >o6.s
cmp -s o6.s lines.exp || pc_fail "ar -p a f1 f3 did not write exactly those two members"
ar -pv a f2 >o3 2>err </dev/null || pc_fail "ar -pv a f2: nonzero status"
[ -f o3 ] || pc_fail "no output file"
cmp -s o3 v2.exp || pc_fail "ar -pv a f2 output differs from the specified header and contents"
ar -pv a >o4 2>err </dev/null || pc_fail "ar -pv a: nonzero status"
[ -f o4 ] || pc_fail "no output file"
cmp -s o4 vall.exp || pc_fail "ar -pv a output differs from the specified headers and contents"
pc_pass
