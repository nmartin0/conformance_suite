# default data specification
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.prs.001
# With no -r, prs writes the SCCS pathname and then, for each delta, the
# delta line, line statistics, MR and comment headings.
. "$PC_CORE/pc.sh"
pc_begin XCU.prs.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
rm -f f
pc_test
prs s.f </dev/null >out 2>err || pc_fail "prs: nonzero status"
head -n 1 out | grep -E 's\.f:?$' >/dev/null || pc_fail "the first line is not the SCCS pathname: $(head -n 1 out)"
grep -E '^D 1\.1 .*[[:blank:]][0-9]+/[0-9]+/[0-9]+$' out >/dev/null || pc_fail "no delta line followed by the line statistics: $(cat out)"
grep -x 'MRs:' out >/dev/null || pc_fail "no MRs: heading"
grep -x 'COMMENTS:' out >/dev/null || pc_fail "no COMMENTS: heading"
pc_pass
