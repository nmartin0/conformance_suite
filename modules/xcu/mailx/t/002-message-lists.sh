# message list forms
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.002
# A message list can be a number, a range n-m, several numbers, * for all, $
# for the last, ^ for the first, + and - for the next and previous message,
# /string for a Subject match ignoring case, or an address.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.002
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
p 1-2
echo @@b@@
p $
echo @@c@@
p *
echo @@d@@
p /SECOND
echo @@e@@
p ADDR
echo @@f@@
p 1 3
echo @@g@@
p ^
echo @@h@@
p 2
echo @@i@@
p +
echo @@j@@
p -
EOC
# the address form must be one shown in the header summary
printf 'set noprompt\nfrom 1\n' >c0
mailx -n -N -f in.mbox <c0 >o0 2>/dev/null || pc_unresolved "setup: cannot get the header summary"
addr=$(awk '{ for (i = 1; i <= NF; i++) if (index($i, "alice") > 0) { print $i; exit } }' o0)
[ -n "$addr" ] || pc_unresolved "setup: no author shown in the header summary"
sed "s|ADDR|$addr|" cmds >cmds2 && mv cmds2 cmds || pc_unresolved "setup: cannot prepare the commands"
pc_test
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
want() { # section tokens that must be present, then absent
	s=$1; shift
	for t in $*; do has $s BODY-$t || pc_fail "section $s: BODY-$t missing: $(sect $s)"; done
	for t in ONE TWO THREE; do
		case " $* " in *" $t "*) ;; *) has $s BODY-$t && pc_fail "section $s: unexpected BODY-$t" ;; esac
	done
}
want a ONE TWO
want b THREE
want c ONE TWO THREE
want d TWO
want e ONE
want f ONE THREE
want g ONE
want h TWO
want i THREE
want j TWO
l1=$(sect c | grep -n BODY-ONE | sed -n 1p | cut -d: -f1)
l2=$(sect c | grep -n BODY-TWO | sed -n 1p | cut -d: -f1)
l3=$(sect c | grep -n BODY-THREE | sed -n 1p | cut -d: -f1)
{ [ "$l1" -lt "$l2" ] && [ "$l2" -lt "$l3" ]; } || pc_fail "* did not write the messages in order"
pc_pass
