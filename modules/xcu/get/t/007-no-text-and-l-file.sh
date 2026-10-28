# -g and -l
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.get.007
# -g retrieves no text but succeeds only for an existing SID, and -l writes
# an l-file listing the applied delta.
. "$PC_CORE/pc.sh"
pc_begin XCU.get.007
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
rm -f f
pc_test
get -g s.f </dev/null >out 2>err || pc_fail "get -g: nonzero status"
[ -e f ] && pc_fail "-g still created a g-file"
get -g -s -r1.1 s.f </dev/null >out 2>err || pc_fail "get -g of the existing SID 1.1: nonzero status"
get -g -s -r1.9 s.f </dev/null >out 2>err
[ "$?" -gt 0 ] || pc_fail "get -g of the nonexistent SID 1.9 gave status 0"
get -l -g -s s.f </dev/null >out 2>err || pc_fail "get -l -g: nonzero status"
[ -f l.f ] || pc_fail "no l-file was created"
grep -E '^...[[:blank:]]1\.1[[:blank:]]' l.f >/dev/null || pc_fail "the l-file has no line for delta 1.1: $(cat l.f)"
pc_pass
