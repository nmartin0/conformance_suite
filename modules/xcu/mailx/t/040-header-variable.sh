# the header variable
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.040
# The header summary is written on entering Receive Mode by default, and
# not when header is unset in the start-up file.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.040
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
printf 'set noheader\n' >rc || pc_unresolved "setup: cannot write the start-up file"
printf 'echo @@a@@\n' >cmds
pc_test
mailx -n -f in.mbox <cmds >out1 2>err || pc_fail "mailx (default): nonzero status"
awk '/@@a@@/ { exit } { print }' out1 >pre1
grep alice pre1 >/dev/null || pc_fail "no initial summary by default"
MAILRC=$(pwd)/rc; export MAILRC
mailx -n -f in.mbox <cmds >out2 2>err || pc_fail "mailx (noheader): nonzero status"
awk '/@@a@@/ { exit } { print }' out2 >pre2
grep -e alice -e carol -e dave pre2 >/dev/null && pc_fail "noheader did not stop the initial summary"
pc_pass
