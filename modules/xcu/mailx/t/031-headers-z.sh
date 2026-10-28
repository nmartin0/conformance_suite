# headers with a message, and z
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.031
# With screen set to one line, headers n writes just the page holding message
# n and makes that message current, headers alone leaves the current message
# alone, and z, z+ and z- scroll the header display one page forward or
# back.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.031
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
cat >cmds <<'EOC'
set noprompt
set screen=1
echo @@a@@
headers 2
echo @@b@@
=
echo @@c@@
headers 1
echo @@d@@
z
echo @@e@@
z+
echo @@f@@
z-
echo @@g@@
p 3
headers
echo @@h@@
=
echo @@i@@
headers 1 3
EOC
pc_test
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
only() { # section, author that must appear, authors that must not
	s=$1; w=$2; shift 2
	has $s $w || pc_fail "section $s: no summary line for $w: $(sect $s)"
	for o in "$@"; do has $s $o && pc_fail "section $s: unexpected summary line for $o"; done
	return 0
}
only a carol alice dave
sect b | grep -w 2 >/dev/null || pc_fail "headers 2 did not make message 2 current: $(sect b)"
only c alice carol dave
only d carol alice dave
only e dave alice carol
only f carol alice dave
sect h | grep -w 3 >/dev/null || pc_fail "headers with no argument changed the current message: $(sect h)"
only i alice carol dave
pc_pass
