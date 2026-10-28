# the MAILRC start-up file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.025
# Commands in the file named by MAILRC are executed at start-up, and a blank
# line in it is ignored.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.025
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
printf 'set noprompt\n\nset pcv_rc1=rcvalue\n\nset pcv_rc2\n' >rc || pc_unresolved "setup: cannot write the start-up file"
MAILRC=$(pwd)/rc; export MAILRC
printf 'echo @@a@@\nset\n' >cmds
pc_test
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
grep -Fx '@@a@@' out >/dev/null || pc_fail "set noprompt in the start-up file had no effect"
has a pcv_rc1 || pc_fail "the first variable of the start-up file is not set: $(sect a)"
has a rcvalue || pc_fail "the value from the start-up file is not set"
has a pcv_rc2 || pc_fail "the variable after the blank line is not set"
pc_pass
