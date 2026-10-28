# a pattern that matches no member
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.014
# A pattern operand that matches no member gives a diagnostic and a nonzero
# exit status, and nothing is extracted.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.014
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'x\n' >f1
pax -w f1 >a 2>/dev/null </dev/null || pc_unresolved "setup: pax -w failed"
rm -f f1
pc_test
pax -r nosuchmember <a >o 2>err
rc=$?
[ "$rc" -ne 0 ] || pc_fail "an unmatched pattern gave status 0"
[ -s err ] || pc_fail "no diagnostic for the unmatched pattern"
[ -e f1 ] && pc_fail "a member not matching the pattern was extracted"
pc_pass
