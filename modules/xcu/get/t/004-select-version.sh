# -r, -t and -c
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.get.004
# -r picks a version, -t takes the top delta of a release or level, and a
# late cutoff with -c keeps every delta.
. "$PC_CORE/pc.sh"
pc_begin XCU.get.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
rm -f f
get -e -s s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: get -e failed"
printf 'four\n' >>f || pc_unresolved "setup: cannot edit f"
delta -s -y'add four' s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: delta failed"
get -e -s s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: second get -e failed"
printf 'five\n' >>f || pc_unresolved "setup: cannot edit f"
delta -s -y'add five' s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: second delta failed"
pc_test
get -s -p -r1.1 s.f </dev/null >out 2>err || pc_fail "get -r1.1: nonzero status"
printf '%b' 'one\ntwo\nthree\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "-r1.1: $(od -c out | head -3)"
get -s -p -r1.2 s.f </dev/null >out 2>err || pc_fail "get -r1.2: nonzero status"
printf '%b' 'one\ntwo\nthree\nfour\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "-r1.2: $(od -c out | head -3)"
get -s -p -t -r1 s.f </dev/null >out 2>err || pc_fail "get -t -r1: nonzero status"
printf '%b' 'one\ntwo\nthree\nfour\nfive\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "-t -r1: $(od -c out | head -3)"
get -s -p -t -r1.2 s.f </dev/null >out 2>err || pc_fail "get -t -r1.2: nonzero status"
printf '%b' 'one\ntwo\nthree\nfour\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "-t -r1.2: $(od -c out | head -3)"
get -s -p -c68 s.f </dev/null >out 2>err || pc_fail "get -c68: nonzero status"
printf '%b' 'one\ntwo\nthree\nfour\nfive\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "-c68: $(od -c out | head -3)"
pc_pass
