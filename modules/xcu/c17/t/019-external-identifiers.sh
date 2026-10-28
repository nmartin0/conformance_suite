# 4095 external identifiers
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.c17.019
# A translation unit with 4095 external identifiers compiles, links and runs.
. "$PC_CORE/pc.sh"
pc_begin XCU.c17.019
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
awk 'BEGIN {
	for (i = 1; i <= 4094; i++) printf "int pcv%d = %d;\n", i, i
	print "int main(void) { return (pcv1 + pcv4094 == 4095 && pcv2000 == 2000) ? 0 : 1; }"
}' </dev/null >big.c
pc_test
c17 -o p big.c </dev/null >out 2>err || pc_fail "a translation unit with 4095 external identifiers was rejected"
./p </dev/null
[ "$?" -eq 0 ] || pc_fail "the program with 4095 external identifiers gave the wrong result"
pc_pass
