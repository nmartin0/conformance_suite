# the keep variable
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.043
# When every message of a secondary mailbox has been deleted, quitting removes
# the file by default and truncates it to zero length when keep is set.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.043
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
mkcmd 'd *' 'q' >cmds
cp orig.mbox in.mbox
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
[ -e in.mbox ] && pc_fail "an emptied mailbox was not removed by default"
mkcmd 'set keep' 'd *' 'q' >cmds
cp orig.mbox in.mbox
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx (keep): nonzero status"
[ -e in.mbox ] || pc_fail "with keep set an emptied mailbox was removed"
[ -s in.mbox ] && pc_fail "with keep set the emptied mailbox is not of zero length"
pc_pass
