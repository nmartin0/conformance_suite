# default g-file and report
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.get.002
# get writes the SID and line count to standard output and creates a read-
# only g-file with the retrieved text.
. "$PC_CORE/pc.sh"
pc_begin XCU.get.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
rm -f f
pc_test
get s.f </dev/null >out 2>err || pc_fail "get: nonzero status"
printf '1.1\n3 lines\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "the report differs: $(cat out)"
[ -f f ] || pc_fail "no g-file was created"
printf 'one\ntwo\nthree\n' >exp
cmp f exp >/dev/null 2>&1 || pc_fail "the g-file text differs"
case $(ls -l f | cut -c2-10) in
*w*) pc_fail "the g-file has write permission: $(ls -l f | cut -c1-10)" ;;
esac
pc_pass
