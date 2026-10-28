# admin -i name creates a read-only SCCS file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.admin.001
# admin -iname creates s.name from the text of a file as the first delta,
# the new file has no write permission, no x-file or z-file is left, and get
# returns the same text.
. "$PC_CORE/pc.sh"
pc_begin XCU.admin.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
command -v get >/dev/null 2>&1 || pc_unresolved "setup: get is not available"
pc_test
admin -if s.f </dev/null >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "admin -if: exit status $rc"
[ -f s.f ] || pc_fail "s.f was not created"
case $(ls -l s.f | cut -c2-10) in
*w*) pc_fail "the new SCCS file has write permission: $(ls -l s.f | cut -c1-10)" ;;
esac
[ -e x.f ] && pc_fail "the x-file x.f was left behind"
[ -e z.f ] && pc_fail "the lock file z.f was left behind"
get -s -p s.f </dev/null >got 2>err || pc_fail "get cannot read the new SCCS file"
cmp got f >/dev/null 2>&1 || pc_fail "the first delta does not hold the text of f"
pc_pass
