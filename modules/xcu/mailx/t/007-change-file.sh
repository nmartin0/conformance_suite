# file, folder, + and # names, and -f with no operand
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.007
# file switches to another mailbox file, # names the previous file, + names
# a file in the folder directory, file with no argument says which mailbox
# is open, -f with no operand reads the mbox, and -i is accepted.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.007
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
cat >other.mbox <<'EOM'
From erin@example.com Thu Jan  4 00:00:00 2024
From: erin@example.com
To: bob@example.com
Subject: other

BODY-OTHER line

EOM
mkdir fold || pc_unresolved "setup: cannot make a directory"
cp other.mbox fold/fb || pc_unresolved "setup: cannot copy the mailbox"
cp orig.mbox dflt || pc_unresolved "setup: cannot copy the mailbox"
{
	printf 'set noprompt\n'
	printf 'file other.mbox\n'
	printf 'echo @@a@@\n'
	printf 'p *\n'
	printf 'echo @@b@@\n'
	printf 'file\n'
	printf 'echo @@c@@\n'
	printf 'file #\n'
	printf 'p 1\n'
	printf 'echo @@d@@\n'
	printf 'set folder=%s/fold\n' "$(pwd)"
	printf 'file +fb\n'
	printf 'p *\n'
} >cmds
pc_test
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
has a BODY-OTHER || pc_fail "file did not open the other mailbox: $(sect a)"
has a BODY-ONE && pc_fail "the old mailbox is still shown after file"
has b other.mbox || pc_fail "file with no argument does not name the open mailbox: $(sect b)"
has c BODY-ONE || pc_fail "file # did not return to the previous file: $(sect c)"
has c BODY-OTHER && pc_fail "file # still shows the other mailbox"
has d BODY-OTHER || pc_fail "file +fb did not open the file in the folder directory: $(sect d)"
# -f with no operand reads the mbox; -i is accepted
printf 'set noprompt\np *\n' >c2
mailx -n -N -f <c2 >o2 2>err || pc_fail "mailx -f with no file: nonzero status"
grep BODY-TWO o2 >/dev/null || pc_fail "-f with no file did not read the mbox"
mailx -n -N -i -f in.mbox <c2 >o3 2>err || pc_fail "mailx -i: nonzero status"
grep BODY-THREE o3 >/dev/null || pc_fail "mailx -i did not read the mailbox"
pc_pass
