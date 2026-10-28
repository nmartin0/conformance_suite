# print, type, Print and Type
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.001
# print and type (with their abbreviations) write the named message to
# standard output without the other messages, and Print and Type write it
# with all of its header fields.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.001
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
echo @@a@@
p 2
echo @@b@@
t 2
echo @@c@@
print 2
echo @@d@@
type 2
echo @@e@@
P 2
echo @@f@@
T 2
EOC
pc_test
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
for s in a b c d; do
	has $s BODY-TWO || pc_fail "section $s: message 2 not written: $(sect $s)"
	has $s BODY-ONE && pc_fail "section $s: message 1 written as well"
	has $s BODY-THREE && pc_fail "section $s: message 3 written as well"
done
for s in e f; do
	has $s BODY-TWO || pc_fail "section $s: message 2 not written"
	has $s 'Subject: second' || pc_fail "section $s: the Subject header field is missing"
	has $s 'To: bob@example.com' || pc_fail "section $s: the To header field is missing"
	has $s BODY-ONE && pc_fail "section $s: message 1 written as well"
done
pc_pass
