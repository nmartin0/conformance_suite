# pax -i: prompts on the terminal, blank skips, "." keeps, another name renames
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.022
# "pax -r -i" on a terminal writes a prompt naming each member to the
# terminal (not to standard error); a blank answer skips the member, a
# single period extracts it under its own name, any other answer is the new
# name; end-of-file in an answer ends pax at once with a nonzero status.
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.pax.022
LC_ALL=C; export LC_ALL
pc_pty_need
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
for n in 1 2 3 4; do printf 'data%s\n' "$n" >f$n || pc_unresolved "setup: cannot write f$n"; done
pax -w -f a.tar f1 f2 f3 f4 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: pax -w failed"
pax -w -f b.tar f1 f2 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: pax -w failed"
rm -f f1 f2 f3 f4
cat >inner1.sh <<'IN'
pax -r -i -f a.tar >i1.out 2>i1.err
echo $? >i1.rc
exit 0
IN
cat >inner2.sh <<'IN'
pax -r -i -f b.tar >i2.out 2>i2.err
echo $? >i2.rc
exit 0
IN
printf '\n.\nrenamed3\n.\n' >typed1
printf '.\n\004' >typed2
pc_test
pc_pty_run -i typed1 inner1.sh
[ "$(cat i1.rc 2>/dev/null)" = 0 ] || pc_fail "pax -r -i exit status '$(cat i1.rc 2>/dev/null)'"
for n in 1 2 3 4; do
	grep -F "f$n" pty.out >/dev/null || pc_fail "no prompt on the terminal names f$n"
done
grep -F f1 i1.err >/dev/null && pc_fail "a prompt naming f1 was written to standard error"
[ -e f1 ] && pc_fail "f1 was extracted although the answer was blank"
[ "$(cat f2 2>/dev/null)" = data2 ] || pc_fail "f2 was not extracted under its own name after the answer \".\""
[ -e f3 ] && pc_fail "f3 was extracted under its own name although another name was typed"
[ "$(cat renamed3 2>/dev/null)" = data3 ] || pc_fail "f3 was not extracted as renamed3"
[ "$(cat f4 2>/dev/null)" = data4 ] || pc_fail "f4 was not extracted after the answer \".\""
rm -f f1 f2 f3 f4 renamed3 pty.out
pc_pty_run -i typed2 inner2.sh
rc=$(cat i2.rc 2>/dev/null)
case $rc in
''|*[!0-9]*) pc_fail "pax -r -i did not finish after end-of-file on the terminal" ;;
esac
[ "$rc" -ne 0 ] || pc_fail "end-of-file in an answer gave exit status 0"
[ -e f2 ] && pc_fail "f2 was extracted after end-of-file in the answer for it"
pc_pass
