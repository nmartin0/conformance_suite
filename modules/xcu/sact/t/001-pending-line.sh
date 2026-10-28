# one line per pending edit
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sact.001
# After get -e, sact writes a line with the SID read, the new SID, a login,
# a date and a time.
. "$PC_CORE/pc.sh"
pc_begin XCU.sact.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
rm -f f
get -e -s s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: get -e failed"
pc_test
sact s.f </dev/null >out 2>err || pc_fail "sact: nonzero status"
n=$(wc -l <out); n=$((n + 0))
[ "$n" -eq 1 ] || pc_fail "sact wrote $n lines, expected 1: $(cat out)"
awk '{ ok = (NF == 5 && $1 == "1.1" && $2 == "1.2" && $4 ~ /^[0-9][0-9]\/[0-9][0-9]\/[0-9][0-9]$/ && $5 ~ /^[0-9][0-9]:[0-9][0-9]:[0-9][0-9]$/) } END { exit !ok }' out || pc_fail "the line does not have the specified fields: $(cat out)"
pc_pass
