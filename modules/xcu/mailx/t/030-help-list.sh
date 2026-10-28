# help, ? and list
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.030
# help and ? write a summary of the commands and list writes the names of
# the available commands, all to standard output.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.030
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
mkcmd 'echo @@a@@' 'list' 'echo @@b@@' 'help' 'echo @@c@@' '?' >cmds
pc_test
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
[ -n "$(sect b)" ] || pc_fail "help wrote nothing to standard output"
[ -n "$(sect c)" ] || pc_fail "? wrote nothing to standard output"
for pair in delete:d[elete] print:p[rint] quit:q[uit] save:s[ave] undelete:u[ndelete] unset:uns[et] headers:h[eaders] echo:ec[ho] source:so[urce] retain:ret[ain] ignore:ig[nore] pipe:pi[pe]; do
	full=${pair%%:*}; br=${pair#*:}
	sect a | grep -F -e "$full" -e "$br" >/dev/null || pc_fail "list does not name $full: $(sect a)"
done
pc_pass
