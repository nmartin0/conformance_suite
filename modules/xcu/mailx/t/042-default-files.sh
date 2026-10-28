# default start-up file and mbox
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.042
# With MAILRC unset the start-up file is .mailrc in HOME, and with MBOX unset
# the mbox is the file mbox in HOME.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.042
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
unset MAILRC MBOX
cp orig.mbox mbox || pc_unresolved "setup: cannot make the mbox"
printf 'set noprompt\nset pcv_def=1\n' >.mailrc || pc_unresolved "setup: cannot write .mailrc"
printf 'echo @@a@@\nset\np 2\n' >cmds
pc_test
mailx -n -N -f <cmds >out 2>err || pc_fail "mailx: nonzero status"
grep -Fx '@@a@@' out >/dev/null || pc_fail ".mailrc in HOME was not used as the start-up file"
has a pcv_def || pc_fail "the variable set in .mailrc is not defined: $(sect a)"
has a BODY-TWO || pc_fail "-f without a file did not read the file mbox in HOME"
pc_pass
