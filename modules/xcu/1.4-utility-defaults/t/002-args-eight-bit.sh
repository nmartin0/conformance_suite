# Utilities process command-line arguments containing arbitrary non-NUL bytes (including 0x01, 0x7f, 0x80 and 0xff) without altering them
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.4-utility-defaults.002
. "$PC_CORE/pc.sh"
pc_begin XCU.1.4-utility-defaults.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
a=$(printf 'a\200\377\001\177z') || pc_unresolved "cannot build the test string"
d=$(printf '\377') || pc_unresolved "cannot build the delimiter"
printf 'a\200\377\001\177z\n' >want || pc_unresolved "cannot write the expected output"
printf '%s\n' "$a" >gf || pc_unresolved "cannot write the grep input"
printf 'x\377y\n' >cf || pc_unresolved "cannot write the cut input"
printf 'p\377q\n' >tf || pc_unresolved "cannot write the tr input"
pc_test
printf '%s\n' "$a" >o1
cmp -s o1 want || pc_fail "printf changed the argument"
basename "/x/$a" >o2
cmp -s o2 want || pc_fail "basename changed the argument"
dirname "$a/y" >o3
cmp -s o3 want || pc_fail "dirname changed the argument"
n=$(expr "$a" : '.*')
[ "$n" = 6 ] || pc_fail "expr counted $n bytes in a six-byte argument"
y=$(cut -d "$d" -f 2 <cf)
[ "$y" = y ] || pc_fail "cut -d with a byte 0xff delimiter gave '$y'"
c=$(grep -F -c -- "$a" gf)
[ "$c" = 1 ] || pc_fail "grep -F did not match the argument (count '$c')"
t=$(tr "$d" X <tf)
[ "$t" = pXq ] || pc_fail "tr with a byte 0xff operand gave '$t'"
pc_pass
