# the read state and the :r message type
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.020
# A message shown by print or top is in state read, and :r in a message list
# selects the read messages.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.020
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
mkcmd 'p 1' 'echo @@a@@' 'p :r' 'echo @@x@@' 'top 3' 'echo @@b@@' 'p :r' >cmds
pc_test
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
has a BODY-ONE || pc_fail "p :r after p 1 did not show message 1: $(sect a)"
has a BODY-TWO && pc_fail "p :r showed message 2, which was not read"
has a BODY-THREE && pc_fail "p :r showed message 3, which was not read"
has b BODY-ONE || pc_fail "p :r after top 3 lost message 1: $(sect b)"
has b BODY-THREE || pc_fail "p :r after top 3 did not show message 3"
has b BODY-TWO && pc_fail "p :r showed message 2, which was not read"
pc_pass
