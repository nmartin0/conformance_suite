# strings made by get
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.what.005
# Text retrieved with the %W% keyword is found by what.
. "$PC_CORE/pc.sh"
pc_begin XCU.what.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'v %%W%%\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
rm -f f
pc_test
get -s -p s.f </dev/null >got 2>err || pc_unresolved "get could not retrieve the text"
what got </dev/null >out 2>err || pc_fail "what: nonzero status"
printf 'got:\n\tf\t1.1\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "what wrote: $(tr '\n' '|' <out)"
pc_pass
