# = and next
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.006
# The current message starts as the first message, = writes its number, a
# command with a list leaves the highest numbered message of the list
# current, and next prints the current message if it has not been shown,
# then each following message in turn.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.006
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
=
echo @@b@@
n
echo @@c@@
n
echo @@d@@
n
echo @@e@@
=
echo @@f@@
p 2
echo @@g@@
=
EOC
pc_test
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
sect a | grep -w 1 >/dev/null || pc_fail "= does not say the current message is 1: $(sect a)"
has b BODY-ONE || pc_fail "the first next did not write message 1: $(sect b)"
has c BODY-TWO || pc_fail "the second next did not write message 2: $(sect c)"
has c BODY-ONE && pc_fail "the second next wrote message 1 again"
has d BODY-THREE || pc_fail "the third next did not write message 3: $(sect d)"
sect e | grep -w 3 >/dev/null || pc_fail "= does not say 3 after three next commands: $(sect e)"
sect g | grep -w 2 >/dev/null || pc_fail "= does not say 2 after p 2: $(sect g)"
pc_pass
