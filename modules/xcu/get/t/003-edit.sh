# -e
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.get.003
# -e reports the new delta SID, creates a writable g-file and a p-file, and
# blocks a second get -e on the same SID.
. "$PC_CORE/pc.sh"
pc_begin XCU.get.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
rm -f f
pc_test
get -e s.f </dev/null >out 2>err || pc_fail "get -e: nonzero status"
printf '1.1\nnew delta 1.2\n3 lines\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "the report differs: $(cat out)"
[ -f f ] || pc_fail "no g-file was created"
case $(ls -l f | cut -c3) in
w) ;;
*) pc_fail "the g-file is not writable by its owner: $(ls -l f | cut -c1-10)" ;;
esac
[ -f p.f ] || pc_fail "no p-file was created"
get -e -s s.f </dev/null >out2 2>err2
rc=$?
[ "$rc" -gt 0 ] || pc_fail "a second get -e on the same SID gave status 0"
pc_pass
