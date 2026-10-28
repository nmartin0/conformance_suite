# crt and PAGER on a terminal
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.035
# When standard output is a terminal and crt is less than the number of
# lines of a message, the message is written through the command named by
# PAGER; with a larger crt it is written directly.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.035
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
MAILRC=/dev/null; HOME=$(pwd); MBOX=$(pwd)/dflt; DEAD=/dev/null; PAGER=cat
export MAILRC HOME MBOX DEAD PAGER
cat >in.mbox <<'EOM'
From alice@example.com Mon Jan  1 00:00:00 2024
From: alice@example.com
To: bob@example.com
Subject: first

BODY-ONE line
more of BODY-ONE

From carol@example.com Tue Jan  2 00:00:00 2024
From: carol@example.com
To: bob@example.com
Subject: second

BODY-TWO line

From dave@example.com Wed Jan  3 00:00:00 2024
From: dave@example.com
To: bob@example.com
Subject: third

BODY-THREE line

EOM
cp in.mbox orig.mbox || pc_unresolved "setup: cannot copy the mailbox"
sect() { awk -v m="@@$1@@" 'index($0, "@@") > 0 { on = (index($0, m) > 0); next } on' out; }
has() { sect "$1" | grep -F -e "$2" >/dev/null; }
mkcmd() { printf 'set noprompt\n'; for l in "$@"; do printf '%s\n' "$l"; done; }
. "$PC_CORE/pcpty.sh"
pc_pty_need
shpath=$(command -v sh) || pc_unresolved "setup: no sh found"
case $shpath in /*) ;; *) pc_unresolved "setup: no absolute path for sh" ;; esac
{
	printf '#!%s\n' "$shpath"
	printf 'cat >>"%s"\n' "$(pwd)/pager.log"
	printf 'printf "PAGER-END\\n" >>"%s"\n' "$(pwd)/pager.log"
} >pg || pc_unresolved "setup: cannot write the pager"
chmod +x pg || pc_unresolved "setup: cannot make the pager executable"
PAGER=$(pwd)/pg; export PAGER
mkcmd 'p 3' 'set crt=3' 'p 1' 'set crt=1000' 'p 2' >cmds
cat >inner.sh <<'IN'
mailx -n -N -f in.mbox <cmds
IN
pc_test
pc_pty_run inner.sh
[ -f pager.log ] || pc_fail "the pager was never run for a message longer than crt"
grep BODY-ONE pager.log >/dev/null || pc_fail "message 1 did not go through the pager"
grep BODY-THREE pager.log >/dev/null && pc_fail "with crt unset message 3 went through the pager"
grep BODY-THREE pty.out >/dev/null || pc_fail "with crt unset message 3 was not written to the terminal"
grep BODY-TWO pager.log >/dev/null && pc_fail "message 2, shorter than crt, went through the pager"
grep BODY-TWO pty.out >/dev/null || pc_fail "message 2 was not written to the terminal"
grep BODY-ONE pty.out >/dev/null && pc_fail "message 1 was written to the terminal as well as the pager"
pc_pass
