# folders and LISTER
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.033
# folders writes the names of the files in the folder directory, using the
# command named by LISTER (ls when it is null).
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.033
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
mkdir fld || pc_unresolved "setup: cannot make the folder directory"
: >fld/pcbox1 && : >fld/pcbox2 || pc_unresolved "setup: cannot make files"
LISTER=ls; export LISTER
mkcmd 'set folder=fld' 'echo @@a@@' 'folders' >cmds
pc_test
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
has a pcbox1 || pc_fail "folders did not list pcbox1: $(sect a)"
has a pcbox2 || pc_fail "folders did not list pcbox2: $(sect a)"
LISTER=; export LISTER
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx (null LISTER): nonzero status"
has a pcbox1 || pc_fail "with a null LISTER, folders did not list pcbox1: $(sect a)"
pc_pass
