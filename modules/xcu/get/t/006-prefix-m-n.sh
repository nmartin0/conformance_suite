# -m and -n
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.get.006
# -m prefixes each line with its SID and a tab, -n with the module name and
# a tab, and both give the module name, SID and text.
. "$PC_CORE/pc.sh"
pc_begin XCU.get.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
rm -f f
pc_test
get -s -p -m s.f </dev/null >out 2>err || pc_fail "get -m: nonzero status"
printf '%b' '1.1\tone\n1.1\ttwo\n1.1\tthree\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "-m output: $(od -c out | head -3)"
get -s -p -n s.f </dev/null >out 2>err || pc_fail "get -n: nonzero status"
printf '%b' 'f\tone\nf\ttwo\nf\tthree\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "-n output: $(od -c out | head -3)"
get -s -p -m -n s.f </dev/null >out 2>err || pc_fail "get -m -n: nonzero status"
printf '%b' 'f\t1.1\tone\nf\t1.1\ttwo\nf\t1.1\tthree\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "-m -n output: $(od -c out | head -3)"
pc_pass
