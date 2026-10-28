# pattern operands, -c and -n
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.003
# With members a1 a2 b1, the pattern 'a*' lists a1 and a2, -c 'a*' lists b1,
# -n 'a*' lists only a1, and "pax -r a2" extracts only a2.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'a\n' >a1; printf 'b\n' >a2; printf 'c\n' >b1
pax -w a1 a2 b1 >arch 2>/dev/null </dev/null || pc_unresolved "setup: pax -w failed"
pc_test
pax 'a*' <arch >o 2>err || pc_fail "pattern list: nonzero status"
[ "$(tr '\n' ' ' <o)" = 'a1 a2 ' ] || pc_fail "pattern a* listed '$(tr '\n' ' ' <o)'"
pax -c 'a*' <arch >o 2>err || pc_fail "-c: nonzero status"
[ "$(tr '\n' ' ' <o)" = 'b1 ' ] || pc_fail "-c a* listed '$(tr '\n' ' ' <o)'"
pax -n 'a*' <arch >o 2>err || pc_fail "-n: nonzero status"
[ "$(tr '\n' ' ' <o)" = 'a1 ' ] || pc_fail "-n a* listed '$(tr '\n' ' ' <o)'"
rm -f a1 a2 b1
pax -r a2 <arch >o 2>err || pc_fail "pax -r a2: nonzero status"
[ -f a2 ] || pc_fail "a2 was not extracted"
[ -e a1 ] || [ -e b1 ] && pc_fail "pax -r a2 extracted other members"
pc_pass
