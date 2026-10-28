# delete, undelete, quit, exit and end-of-file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.003
# Deleted messages are not shown by later commands and are removed from the
# file when mailx quits (by quit or end-of-file) but not by exit; undelete
# restores them; delete with no list deletes the current message.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.003
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
pc_test
# deleted messages vanish from later commands and from the file on quit
mkcmd 'd 2' 'echo @@a@@' 'p *' 'echo @@b@@' 'p 2' 'q' >cmds
# p 2 names a deleted message, which is an error, so the exit status is not checked here
mailx -n -N -f in.mbox <cmds >out 2>err
has a BODY-ONE || pc_fail "message 1 not written after deleting 2"
has a BODY-THREE || pc_fail "message 3 not written after deleting 2"
has a BODY-TWO && pc_fail "deleted message 2 written by p *"
has b BODY-TWO && pc_fail "deleted message 2 written by p 2"
grep BODY-TWO in.mbox >/dev/null && pc_fail "message 2 still in the file after quit"
grep BODY-ONE in.mbox >/dev/null || pc_fail "message 1 lost from the file"
grep BODY-THREE in.mbox >/dev/null || pc_fail "message 3 lost from the file"
[ "$(grep -c '^From ' in.mbox)" -eq 2 ] || pc_fail "the file does not hold two messages after quit"
grep 'Subject: third' in.mbox >/dev/null || pc_fail "message 3 lost its header"
# undelete restores
cp orig.mbox in.mbox
mkcmd 'd 1' 'u 1' 'q' >cmds
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx (undelete): nonzero status"
[ "$(grep -c '^From ' in.mbox)" -eq 3 ] || pc_fail "undelete did not keep message 1"
grep BODY-ONE in.mbox >/dev/null || pc_fail "message 1 lost despite undelete"
# exit does not remove deleted messages
cp orig.mbox in.mbox
mkcmd 'd 1' 'x' >cmds
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx (exit): nonzero status"
cmp orig.mbox in.mbox >/dev/null 2>&1 || pc_fail "exit changed the mailbox file"
# end-of-file acts as quit; delete with no list takes the current message
cp orig.mbox in.mbox
mkcmd 'd 3' >cmds
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx (end-of-file): nonzero status"
grep BODY-THREE in.mbox >/dev/null && pc_fail "end-of-file did not delete message 3"
[ "$(grep -c '^From ' in.mbox)" -eq 2 ] || pc_fail "end-of-file left the wrong number of messages"
cp orig.mbox in.mbox
mkcmd 'd' 'q' >cmds
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx (plain d): nonzero status"
grep BODY-ONE in.mbox >/dev/null && pc_fail "d did not delete the current (first) message"
[ "$(grep -c '^From ' in.mbox)" -eq 2 ] || pc_fail "d removed the wrong number of messages"
pc_pass
