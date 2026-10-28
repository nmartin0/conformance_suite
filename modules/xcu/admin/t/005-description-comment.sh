# -t descriptive text and -y comment
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.admin.005
# -tname stores descriptive text, -t alone removes it from an existing file,
# -y sets the comment of the initial delta, and the default comment starts
# with the creation text.
. "$PC_CORE/pc.sh"
pc_begin XCU.admin.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v prs >/dev/null 2>&1 || pc_unresolved "setup: prs is not available"
printf 'x\n' >f || pc_unresolved "setup: cannot write f"
printf 'desc line\n' >d || pc_unresolved "setup: cannot write d"
pc_test
admin -td -y'first reason' -if s.f </dev/null >out 2>err || pc_fail "admin -td -y: nonzero status"
got=$(prs -r -d':FD:' s.f </dev/null 2>/dev/null)
[ "$got" = 'desc line' ] || pc_fail "the description reads back as '$got'"
got=$(prs -r -d':C:' s.f </dev/null 2>/dev/null)
[ "$got" = 'first reason' ] || pc_fail "the comment reads back as '$got'"
admin -t s.f </dev/null >out 2>err || pc_fail "admin -t (remove): nonzero status"
got=$(prs -r -d':FD:' s.f </dev/null 2>/dev/null)
[ -z "$got" ] || pc_fail "-t alone did not remove the description: '$got'"
admin -td s.f </dev/null >out 2>err || pc_fail "admin -td (replace): nonzero status"
got=$(prs -r -d':FD:' s.f </dev/null 2>/dev/null)
[ "$got" = 'desc line' ] || pc_fail "-td did not restore the description: '$got'"
admin -if s.g </dev/null >out 2>err || pc_fail "admin -if without -y: nonzero status"
got=$(prs -r -d':C:' s.g </dev/null 2>/dev/null)
case $got in
'date and time created '*' by '*) ;;
*) pc_fail "the default comment is '$got'" ;;
esac
pc_pass
