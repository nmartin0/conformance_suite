# alias, group, unalias and alternates
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.012
# alias and group record the addresses of a name, alias with a name lists
# it, alias with no arguments lists all, unalias removes one, and alternates
# stores and lists alternative addresses.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.012
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
alias g1 x@e.com y@e.com
echo @@a@@
alias g1
group g2 z@e.com
echo @@b@@
alias
unalias g1
echo @@c@@
alias
alternates a1@h a2@h
echo @@d@@
alternates
EOC
pc_test
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
has a x@e.com || pc_fail "alias g1 does not list x@e.com: $(sect a)"
has a y@e.com || pc_fail "alias g1 does not list y@e.com"
has b x@e.com || pc_fail "alias with no arguments does not list g1: $(sect b)"
has b z@e.com || pc_fail "group did not define g2: $(sect b)"
has c z@e.com || pc_fail "unalias g1 removed g2: $(sect c)"
has c x@e.com && pc_fail "unalias did not remove g1"
has d a1@h || pc_fail "alternates does not list a1@h: $(sect d)"
has d a2@h || pc_fail "alternates does not list a2@h"
pc_pass
