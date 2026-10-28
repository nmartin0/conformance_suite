# insert and delete counts and the -y comment
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.delta.003
# Replacing one line of three reports one inserted, one deleted and two
# unchanged, and the -y text becomes the delta comment.
. "$PC_CORE/pc.sh"
pc_begin XCU.delta.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
rm -f f
get -e -s s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: get -e failed"
printf 'one\nTWO\nthree\n' >f || pc_unresolved "setup: cannot edit f"
command -v prs >/dev/null 2>&1 || pc_unresolved "setup: prs is not available"
pc_test
delta -y'change two' s.f </dev/null >out 2>err || pc_fail "delta: nonzero status"
printf '1.2\n1 inserted\n1 deleted\n2 unchanged\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "the delta report differs: $(cat out)"
got=$(prs -r1.2 -d':C:' s.f </dev/null 2>/dev/null)
[ "$got" = 'change two' ] || pc_fail "the comment reads back as '$got'"
pc_pass
