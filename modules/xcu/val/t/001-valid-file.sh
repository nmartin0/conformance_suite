# a valid file passes
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.val.001
# val on an intact SCCS file writes nothing and exits 0.
. "$PC_CORE/pc.sh"
pc_begin XCU.val.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
rm -f f
pc_test
val s.f </dev/null >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "val on an intact file: exit status $rc"
[ -s out ] && pc_fail "val wrote a message for an intact file: $(cat out)"
pc_pass
