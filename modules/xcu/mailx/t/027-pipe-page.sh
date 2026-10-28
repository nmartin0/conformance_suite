# the page variable with pipe
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.027
# With page set, pipe inserts a form-feed after each message it sends to the
# command, and by default none.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.027
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
mkcmd 'pipe 1 2 cat>>pg2.out' 'set page' 'pipe 1 2 cat>>pg1.out' >cmds
pc_test
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
for f in pg1.out pg2.out; do
	for t in ONE TWO; do grep BODY-$t $f >/dev/null 2>&1 || pc_fail "$f lacks message $t"; done
done
n1=$(tr -cd '\014' <pg1.out | wc -c | tr -d ' ')
n2=$(tr -cd '\014' <pg2.out | wc -c | tr -d ' ')
[ "$n1" -eq 2 ] || pc_fail "with page set the piped text has $n1 form-feeds, not 2"
[ "$n2" -eq 0 ] || pc_fail "with the default (no page) the piped text has $n2 form-feeds"
pc_pass
