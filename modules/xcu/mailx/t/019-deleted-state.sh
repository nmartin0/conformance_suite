# deleted messages, undelete forms, current message after delete
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.019
# Deleted messages are ignored by message lists (a /string search skips them,
# only undelete can name them with /string, :d or its default choice, and no
# other command shows them); with no list undelete restores the next deleted
# message after the current one, else the last one before it; after a delete
# the current message is the first remaining message after the deleted ones,
# else the one before.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.019
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
isnum() { # section, number: the section names that number and no other of 1-3
	s=$(sect "$1")
	printf '%s\n' "$s" | grep -w "$2" >/dev/null || return 1
	for k in 1 2 3; do
		[ "$k" = "$2" ] && continue
		printf '%s\n' "$s" | grep -w "$k" >/dev/null && return 1
	done
	return 0
}
run() { cp orig.mbox in.mbox; mkcmd "$@" >cmds; mailx -n -N -f in.mbox <cmds >out 2>err; }
pc_test
run 'd 2' 'echo @@a@@' 'p /s' 'echo @@b@@' 'u /second' 'echo @@c@@' 'p 2' 'echo @@d@@' 'd 2' 'echo @@e@@' 'p :d'
has a BODY-ONE || pc_fail "p /s did not show the live message with s in its subject: $(sect a)"
has a BODY-TWO && pc_fail "/s matched the deleted message"
has c BODY-TWO || pc_fail "u /second did not restore the deleted message: $(sect c)"
has e BODY && pc_fail "p :d showed a deleted message: $(sect e)"
run 'd 1 3' 'u :d' 'echo @@a@@' 'p *'
for t in ONE TWO THREE; do has a BODY-$t || pc_fail "u :d did not restore message $t: $(sect a)"; done
run 'd 1 3' 'u' 'echo @@a@@' 'p *'
has a BODY-THREE || pc_fail "u with no list did not restore the deleted message after the current one: $(sect a)"
has a BODY-ONE && pc_fail "u with no list restored the wrong message (1)"
run 'd 1' 'p 3' 'u' 'echo @@a@@' 'p *'
has a BODY-ONE || pc_fail "u with no list did not restore the deleted message before the current one: $(sect a)"
run 'd 2' 'echo @@a@@' '='
isnum a 3 || pc_fail "after d 2 the current message is not 3: $(sect a)"
run 'd 2 3' 'echo @@a@@' '='
isnum a 1 || pc_fail "after d 2 3 the current message is not 1: $(sect a)"
run 'd 1' 'echo @@a@@' '='
isnum a 2 || pc_fail "after d 1 the current message is not 2: $(sect a)"
pc_pass
