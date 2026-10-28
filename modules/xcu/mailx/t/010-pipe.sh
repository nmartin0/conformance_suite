# pipe and the | command
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.010
# pipe and | give the listed messages to a shell command as its standard
# input; with no arguments the current message goes to the command held in
# the cmd variable.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.010
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
cat >cmds <<'EOC'
set noprompt
set cmd='cat >dfltpipe.out'
pipe
pipe 2 'cat >piped2.out'
| 3 'cat >piped3.out'
EOC
pc_test
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
grep BODY-ONE dfltpipe.out >/dev/null 2>&1 || pc_fail "pipe with no arguments did not give the current message to the cmd command"
grep BODY-TWO dfltpipe.out >/dev/null 2>&1 && pc_fail "pipe with no arguments gave more than one message"
grep BODY-TWO piped2.out >/dev/null 2>&1 || pc_fail "pipe 2 did not give message 2 to the command"
grep BODY-ONE piped2.out >/dev/null 2>&1 && pc_fail "pipe 2 gave other messages"
grep BODY-THREE piped3.out >/dev/null 2>&1 || pc_fail "| 3 did not give message 3 to the command"
grep BODY-TWO piped3.out >/dev/null 2>&1 && pc_fail "| 3 gave other messages"
pc_pass
