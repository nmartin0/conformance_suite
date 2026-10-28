# ignore, discard and retain
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.011
# ignore (and discard) hide the named header fields, ignoring case, when
# messages are written with print, Print overrides that, a command without
# names lists the hidden names, and retain shows only the named fields.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.011
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
ignore TO
echo @@a@@
p 2
echo @@b@@
P 2
echo @@c@@
ignore
retain subject
echo @@d@@
p 2
EOC
pc_test
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
has a BODY-TWO || pc_fail "print lost the text: $(sect a)"
has a 'Subject: second' || pc_fail "ignore TO hid the Subject field"
has a 'To: bob@example.com' && pc_fail "ignore TO (upper case) did not hide the To field"
has b 'To: bob@example.com' || pc_fail "Print did not override ignore: $(sect b)"
sect c | grep -i '^[^:]*to' >/dev/null || pc_fail "ignore with no names did not list to: $(sect c)"
has d 'Subject: second' || pc_fail "retain subject hid the Subject field: $(sect d)"
has d 'To: bob@example.com' && pc_fail "retain subject still shows the To field"
has d 'From: carol@example.com' && pc_fail "retain subject still shows the From field"
has d BODY-TWO || pc_fail "retain lost the message text"
pc_pass
