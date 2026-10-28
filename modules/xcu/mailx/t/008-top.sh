# top and toplines
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.008
# top writes only the first few lines of a message, as many as the toplines
# variable says (default 5), so a long message is cut short and a large
# toplines value shows all of it.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.008
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
{
	cat <<'EOM'
From frank@example.com Fri Jan  5 00:00:00 2024
From: frank@example.com
To: bob@example.com
Subject: long

EOM
	i=1
	while [ $i -le 20 ]; do
		printf 'BODY-L%02d\n' $i
		i=$((i + 1))
	done
	printf '\n'
} >long.mbox
cat >cmds <<'EOC'
set noprompt
echo @@a@@
top 1
echo @@b@@
set toplines=100
top 1
echo @@c@@
p 1
EOC
pc_test
mailx -n -N -f long.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
[ -n "$(sect a)" ] || pc_fail "top wrote nothing"
has a BODY-L20 && pc_fail "top with the default toplines wrote the whole message"
has a BODY-L01 && : 
has b BODY-L20 || pc_fail "top with toplines=100 did not reach the end of the message"
has c BODY-L20 || pc_fail "print did not write the end of the message"
na=$(sect a | wc -l | tr -d ' '); nc=$(sect c | wc -l | tr -d ' ')
[ "$na" -lt "$nc" ] || pc_fail "top wrote as many lines as print ($na and $nc)"
pc_pass
