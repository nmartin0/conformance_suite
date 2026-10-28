# autoprint, dp and dt
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.028
# With autoprint set, delete writes the current message and undelete writes
# the last message restored; without it neither writes. dp and dt write the
# message that becomes current when it follows the deleted ones, regardless
# of autoprint, and mark it read.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.028
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
run() { cp orig.mbox in.mbox; mkcmd "$@" >cmds; mailx -n -N -f in.mbox <cmds >out 2>err; }
pc_test
run 'echo @@a@@' 'd 1' 'echo @@b@@'
has a BODY- && pc_fail "delete wrote a message without autoprint: $(sect a)"
run 'set autoprint' 'echo @@a@@' 'd 1' 'echo @@b@@' 'p :r'
has b BODY-TWO || pc_fail "the message written by delete with autoprint is not in state read: $(sect b)"
has a BODY-TWO || pc_fail "delete with autoprint did not write the new current message: $(sect a)"
has a BODY-ONE && pc_fail "delete with autoprint wrote the deleted message"
has a BODY-THREE && pc_fail "delete with autoprint wrote message 3"
run 'set autoprint' 'echo @@a@@' 'd 1 2' 'echo @@b@@' 'u 1 2' 'echo @@c@@'
has a BODY-THREE || pc_fail "d 1 2 with autoprint did not write message 3: $(sect a)"
has b BODY-TWO || pc_fail "undelete with autoprint did not write the last restored message: $(sect b)"
has b BODY-ONE && pc_fail "undelete with autoprint wrote more than the last restored message"
run 'd 1 2' 'echo @@a@@' 'u 1 2' 'echo @@b@@'
has a BODY- && pc_fail "undelete wrote a message without autoprint: $(sect a)"
run 'echo @@a@@' 'dp 1' 'echo @@b@@' 'dp 3' 'echo @@c@@' 'p :r'
has a BODY-TWO || pc_fail "dp 1 did not write message 2: $(sect a)"
has a BODY-THREE && pc_fail "dp 1 wrote message 3"
has a BODY-ONE && pc_fail "dp 1 wrote the deleted message"
has b BODY- && pc_fail "dp 3 wrote a message although the current one precedes it: $(sect b)"
has c BODY-TWO || pc_fail "message 2 written by dp is not in state read: $(sect c)"
has c BODY-ONE && pc_fail "p :r showed a deleted message"
has c BODY-THREE && pc_fail "p :r showed a deleted message"
run 'set autoprint' 'echo @@a@@' 'dp 1' 'echo @@b@@'
[ "$(sect a | grep -c 'BODY-TWO line')" -eq 1 ] || pc_fail "with autoprint dp did not write message 2 exactly once: $(sect a)"
run 'echo @@a@@' 'dt 1' 'echo @@b@@'
has a BODY-TWO || pc_fail "dt 1 did not write message 2: $(sect a)"
pc_pass
