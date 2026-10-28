# names on standard input
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rmdel.005
# A single - operand reads SCCS file names from standard input.
. "$PC_CORE/pc.sh"
pc_begin XCU.rmdel.005
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
pc_test
printf 's.f\n' | rmdel -r1.2 - >out 2>err || pc_fail "rmdel -r1.2 - : nonzero status"
get -s -p  s.f </dev/null >out 2>err || pc_fail "get : nonzero status"
printf '%b' 'one\ntwo\nthree\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "the text after rmdel: $(od -c out | head -3)"
pc_pass
