# prs :MR:, :BD: and :GB:
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.prs.007
. "$PC_CORE/pc.sh"
pc_begin XCU.prs.007
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
for u in get admin prs delta unget val; do command -v $u >/dev/null 2>&1 || pc_notinuse "$u is not available"; done
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
admin -if -fv -m'mrone' s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin"
pc_test
got=$(prs -r1.1 -d':MR:' s.f </dev/null 2>/dev/null)
case $got in *mrone*) ;; *) pc_fail ":MR: reads '$got'" ;; esac
prs -r1.1 -d':BD:' s.f </dev/null >out 2>err || pc_fail "prs :BD: nonzero status: $(cat err)"
for w in one two three; do grep -F "$w" out >/dev/null || pc_fail ":BD: has no line '$w': $(cat out)"; done
got=$(prs -r1.1 -d':GB:' s.f </dev/null 2>/dev/null)
exp='one
two
three'
[ "$got" = "$exp" ] || pc_fail ":GB: reads '$got'"
pc_pass
