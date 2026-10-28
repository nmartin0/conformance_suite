# bc supports an output base of 99, a scale of 99, an array index of 2047 and a string constant of 1000 bytes
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.2-utility-limits.005
. "$PC_CORE/pc.sh"
pc_begin XCU.1.2-utility-limits.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
awk 'BEGIN { printf "\""; for (i = 0; i < 1000; i++) printf "x"; print "\"" }' >str.bc || pc_unresolved "cannot build the input"
awk 'BEGIN { printf "."; for (i = 0; i < 99; i++) printf "3"; print "" }' >third || pc_unresolved "cannot build the input"
pc_test
r=$(printf 'obase=99\n100\n' | bc 2>/dev/null) || pc_fail "bc failed with obase 99"
r=$(echo $r)
[ "$r" = "01 01" ] || pc_fail "obase 99 printed '$r', wanted '01 01'"
printf 'scale=99\n1/3\n' | bc 2>/dev/null | tr -d '\\\n' >got.third || pc_fail "bc failed with scale 99"
echo >>got.third
cmp -s got.third third || pc_fail "scale 99 gave '$(cat got.third)'"
r=$(printf 'a[2047]=5\na[2047]\n' | bc 2>/dev/null) || pc_fail "bc failed with array index 2047"
[ "$r" = 5 ] || pc_fail "array element 2047 gave '$r'"
bc <str.bc >got.str 2>/dev/null || pc_fail "bc failed on a 1000 byte string"
n=$(wc -c <got.str | tr -d ' ')
[ "$n" -ge 1000 ] || pc_fail "the string came back with $n bytes"
pc_pass
