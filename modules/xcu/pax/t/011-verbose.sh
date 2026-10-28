# pax -v in list and write modes
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.011
# "pax -v" in list mode writes an ls -l style line per member (type
# character - for a file and d for a directory, nine or more fields, the
# member name last); in write mode "-v" writes the member names, one per
# line, to standard error.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.011
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\n' >f1; mkdir d; printf 'nested\n' >d/f2
pax -w f1 d >a 2>/dev/null </dev/null || pc_unresolved "setup: pax -w failed"
pc_test
pax -v <a >o 2>err || pc_fail "pax -v: nonzero status"
n=$(wc -l <o); n=$((n + 0))
[ "$n" -eq 3 ] || pc_fail "-v listed $n lines, expected 3"
awk 'NF < 9 { bad = 1 } END { exit bad }' o || pc_fail "a -v line has fewer than nine fields"
awk '{ c = substr($1, 1, 1); if (c != "-" && c != "d") bad = 1 } END { exit bad }' o || pc_fail "a -v line does not start with - or d"
awk '$NF == "f1" && substr($1, 1, 1) == "-" { ok = 1 } END { exit !ok }' o || pc_fail "no regular file line for f1"
awk '($NF == "d" || $NF == "d/") && substr($1, 1, 1) == "d" { ok = 1 } END { exit !ok }' o || pc_fail "no directory line for d"
pax -w -v f1 d >b 2>e </dev/null || pc_fail "pax -w -v: nonzero status"
grep -x 'f1' e >/dev/null || pc_fail "write mode -v did not write f1 to standard error"
grep -x 'd/f2' e >/dev/null || pc_fail "write mode -v did not write d/f2 to standard error"
pc_pass
