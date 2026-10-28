# read messages stay in a secondary mailbox
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.021
# When mailx quits, read messages of a mailbox given with -f stay in that
# file and are not moved to the mbox.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.021
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
mkcmd 'p *' 'top 1' 'q' >cmds
pc_test
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
[ "$(grep -c '^From ' in.mbox)" -eq 3 ] || pc_fail "the file no longer holds three messages after quit"
for t in ONE TWO THREE; do grep BODY-$t in.mbox >/dev/null || pc_fail "message $t lost from the file"; done
if [ -f dflt ] && grep BODY- dflt >/dev/null; then pc_fail "read messages were moved to the mbox"; fi
pc_pass
