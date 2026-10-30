# from, headers, the current-message mark, -H and -N
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.005
# from and headers write one summary line per message that is not deleted,
# the current message line begins with >, -H writes only the summary and
# exits, and -N leaves out the initial summary.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.005
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
from *
echo @@b@@
p 2
echo @@c@@
from *
echo @@d@@
d 2
echo @@e@@
from *
echo @@f@@
h
EOC
pc_test
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
for n in alice carol dave; do
	[ "$(sect a | grep -c "$n")" -eq 1 ] || pc_fail "from * does not give exactly one line for $n: $(sect a)"
done
[ "$(sect a | grep -c '^>')" -eq 1 ] || pc_fail "from * does not mark one current message with >"
sect a | grep '^>' | grep alice >/dev/null || pc_fail "the initial current message is not the first"
[ "$(sect c | grep -c '^>')" -eq 1 ] || pc_fail "after p 2 not exactly one line begins with >"
sect c | grep '^>' | grep carol >/dev/null || pc_fail "after p 2 the > line is not message 2"
has e carol && pc_fail "from * shows the deleted message"
has e alice || pc_fail "from * lost message 1 after the delete"
has e dave || pc_fail "from * lost message 3 after the delete"
has f carol && pc_fail "headers shows the deleted message"
has f alice || pc_fail "headers lost message 1"
has f dave || pc_fail "headers lost message 3"
# -H writes only the summary
mailx -n -H -f in.mbox </dev/null >hout 2>err || pc_fail "mailx -H: nonzero status: $(sed 1q err)"
for n in alice carol dave; do
	grep "$n" hout >/dev/null || pc_fail "-H: no line for $n"
done
grep BODY- hout >/dev/null && pc_fail "-H wrote message text"
grep '^>' hout | grep alice >/dev/null || pc_fail "-H: the first message is not marked current"
# the initial summary is written unless -N is given
printf 'set noprompt\necho @@a@@\n' >c2
mailx -n -f in.mbox <c2 >o2 2>err || pc_fail "mailx without -N: nonzero status"
awk '/@@a@@/ { exit } { print }' o2 >pre2
grep alice pre2 >/dev/null || pc_fail "no initial summary without -N"
mailx -n -N -f in.mbox <c2 >o3 2>err || pc_fail "mailx -N: nonzero status"
awk '/@@a@@/ { exit } { print }' o3 >pre3
grep -e alice -e carol -e dave pre3 >/dev/null && pc_fail "-N still wrote the initial summary"
pc_pass
