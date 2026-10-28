# delta prompts for the comment on a terminal, not otherwise; -y alone is a null comment
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.delta.010
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.delta.010
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
for u in get admin delta prs; do command -v $u >/dev/null 2>&1 || pc_notinuse "$u is not available"; done
pc_pty_need
printf 'one\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin"
get -e -s s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: get -e"
printf 'one\ntwo\n' >f
cat >inner.sh <<'IN'
delta -s s.f 2>d.err
echo $? >d.rc
exit 0
IN
printf 'typed reason\n' >typed
pc_test
delta -s -y s.f </dev/null >out 2>err || pc_fail "delta -y (null comment): nonzero status: $(cat err)"
grep 'comments?' out >/dev/null && pc_fail "a prompt was written although -y was given"
got=$(prs -r1.2 -d':C:' s.f </dev/null 2>/dev/null)
[ -z "$got" ] || pc_fail "the null comment reads back as '$got'"
get -e -s s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: second get -e"
printf 'one\ntwo\nthree\n' >f
delta -s s.f </dev/null >out 2>err || pc_fail "delta without -y, standard input not a terminal: nonzero status: $(cat err)"
grep 'comments?' out >/dev/null && pc_fail "a prompt was written although standard input is not a terminal"
get -e -s s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: third get -e"
printf 'one\ntwo\nthree\nfour\n' >f
pc_pty_run -i typed inner.sh
[ "$(cat d.rc 2>/dev/null)" = 0 ] || pc_fail "delta on a terminal: exit status '$(cat d.rc 2>/dev/null)'"
grep 'comments?' pty.out >/dev/null || pc_fail "no comments? prompt written: $(cat pty.out)"
got=$(prs -r1.4 -d':C:' s.f </dev/null 2>/dev/null)
[ "$got" = 'typed reason' ] || pc_fail "the typed comment reads back as '$got'"
pc_pass
