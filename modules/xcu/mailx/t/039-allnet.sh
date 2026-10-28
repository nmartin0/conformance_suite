# the allnet variable
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.039
# By default an address in a message list selects only messages from exactly
# that address; with allnet set, network names with the same login name
# match.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.039
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
cat >net.mbox <<'EOM'
From alice@example.com Mon Jan  1 00:00:00 2024
From: alice@example.com
To: bob@example.com
Subject: first

BODY-ONE line

From alice@other.example.org Tue Jan  2 00:00:00 2024
From: alice@other.example.org
To: bob@example.com
Subject: second

BODY-TWO line

From dave@example.com Wed Jan  3 00:00:00 2024
From: dave@example.com
To: bob@example.com
Subject: third

BODY-THREE line

EOM
mkcmd 'echo @@a@@' 'p alice@example.com' 'echo @@b@@' 'set allnet' 'p alice@example.com' >cmds
pc_test
mailx -n -N -f net.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
has a BODY-ONE || pc_fail "an address did not select its message: $(sect a)"
has a BODY-TWO && pc_fail "without allnet, another network's address matched"
has b BODY-ONE || pc_fail "with allnet, the exact address did not match: $(sect b)"
has b BODY-TWO || pc_fail "with allnet, the same login name on another network did not match"
has b BODY-THREE && pc_fail "with allnet, a different login name matched"
pc_pass
