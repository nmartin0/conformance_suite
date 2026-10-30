# save, write and copy
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.004
# save and copy append the messages with their headers in the mailbox file
# format, creating the file, with no file operand save goes to the mbox, and
# write puts the message without its header into the file.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.004
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
s 2 saved.mbox
w 3 written.txt
c 1 copied.mbox
s 1 saved.mbox
t 3
s
EOC
pc_test
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
[ -f saved.mbox ] || pc_fail "save did not create the file"
sed -n 1p saved.mbox | grep '^From ' >/dev/null || pc_fail "the saved file does not start with a From line"
[ "$(grep -c '^From ' saved.mbox)" -eq 2 ] || pc_fail "the saved file does not hold two messages"
grep 'Subject: second' saved.mbox >/dev/null || pc_fail "save lost the header of message 2"
grep BODY-TWO saved.mbox >/dev/null || pc_fail "save lost the text of message 2"
grep 'Subject: first' saved.mbox >/dev/null || pc_fail "the second save did not append message 1"
l2=$(grep -n BODY-TWO saved.mbox | sed -n 1p | cut -d: -f1)
l1=$(grep -n BODY-ONE saved.mbox | sed -n 1p | cut -d: -f1)
[ "$l2" -lt "$l1" ] || pc_fail "the second save was not appended after the first"
[ -f written.txt ] || pc_fail "write did not create the file"
grep BODY-THREE written.txt >/dev/null || pc_fail "write lost the text"
grep 'Subject:' written.txt >/dev/null && pc_fail "write kept a header field"
grep 'dave@example.com' written.txt >/dev/null && pc_fail "write kept the header"
[ -f copied.mbox ] || pc_fail "copy did not create the file"
grep 'Subject: first' copied.mbox >/dev/null || pc_fail "copy lost the header"
grep BODY-ONE copied.mbox >/dev/null || pc_fail "copy lost the text"
[ -f dflt ] || pc_fail "save with no file did not write the mbox"
grep BODY-THREE dflt >/dev/null || pc_fail "save with no file lost the text"
grep 'Subject: third' dflt >/dev/null || pc_fail "save with no file lost the header"
pc_pass
