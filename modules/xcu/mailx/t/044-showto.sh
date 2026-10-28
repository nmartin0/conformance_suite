# the showto variable
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.044
# By default the header summary line shows the author of a message; with
# showto set, a message sent by the invoking user shows the To field
# instead.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.044
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
me=$(id -un) || pc_unresolved "setup: cannot find the user name"
case $me in *[!A-Za-z0-9_.-]*|'') pc_unresolved "setup: unusual user name" ;; esac
cat >mine.mbox <<EOM
From $me Mon Jan  1 00:00:00 2024
From: $me
To: pcrecipient@example.com
Subject: mine

BODY-MINE line

EOM
mkcmd 'echo @@a@@' 'from 1' 'set showto' 'echo @@b@@' 'from 1' >cmds
pc_test
mailx -n -N -f mine.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
has a "$me" || pc_fail "the default summary does not show the author: $(sect a)"
has a pcrecipient && pc_fail "the default summary shows the To field"
has b pcrecipient || pc_fail "with showto the summary does not show the To field: $(sect b)"
pc_pass
