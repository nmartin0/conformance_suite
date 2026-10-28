# Save and Copy name the file after the author
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.036
# Save and Copy write the messages, with their headers, to a file named for
# the author of the first message with the network address removed.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.036
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
mkcmd 'ignore subject' 'S 1 3' 'C 2' >cmds
pc_test
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
grep BODY-ONE alice >/dev/null 2>&1 || pc_fail "S 1 3 did not write message 1 to the file alice"
grep BODY-THREE alice >/dev/null 2>&1 || pc_fail "S 1 3 did not write message 3 to the file alice"
grep BODY-TWO alice >/dev/null 2>&1 && pc_fail "S 1 3 wrote message 2"
grep 'Subject: first' alice >/dev/null 2>&1 || pc_fail "S 1 3 left out a header field that is ignored for display"
[ -e dave ] && pc_fail "S 1 3 made a file for the author of message 3"
grep BODY-TWO carol >/dev/null 2>&1 || pc_fail "C 2 did not write message 2 to the file carol"
grep 'Subject: second' carol >/dev/null 2>&1 || pc_fail "C 2 did not keep the header fields"
[ -e alice@example.com ] && pc_fail "the file name kept the network address"
pc_pass
