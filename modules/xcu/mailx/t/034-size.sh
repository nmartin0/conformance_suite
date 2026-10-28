# size
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.034
# size writes the size in bytes of a message: two messages whose headers have
# the same length and whose bodies differ by 96 bytes differ in size by 96.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.034
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
body2=$(printf '%0100d' 0 | tr 0 B)
{
	printf 'From x@example.com Mon Jan  1 00:00:00 2024\nFrom: x@example.com\nTo: y@example.com\nSubject: s\n\nAAAA\n\n'
	printf 'From x@example.com Mon Jan  1 00:00:00 2024\nFrom: x@example.com\nTo: y@example.com\nSubject: s\n\n%s\n\n' "$body2"
} >sz.mbox || pc_unresolved "setup: cannot write the mailbox"
mkcmd 'echo @@a@@' 'size 1' 'echo @@b@@' 'size 2' >cmds
maxnum() { sect "$1" | tr -c '0-9\n' ' ' | tr ' ' '\n' | grep '^[0-9]' | sort -n | tail -n 1; }
pc_test
mailx -n -N -f sz.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
s1=$(maxnum a); s2=$(maxnum b)
[ -n "$s1" ] && [ -n "$s2" ] || pc_fail "size wrote no number: $(sect a) / $(sect b)"
[ $((s2 - s1)) -eq 96 ] || pc_fail "the sizes $s1 and $s2 do not differ by the 96 bytes of body"
pc_pass
