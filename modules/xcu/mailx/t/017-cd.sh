# cd and chdir
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.017
# cd and chdir change the directory of later relative file names; with no
# argument they go to the directory named by HOME.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.017
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
mkdir sub || pc_unresolved "setup: cannot make a directory"
cat >cmds <<'EOC'
set noprompt
cd sub
save 1 inside.out
cd
save 2 home.out
chdir sub
save 3 ch.out
EOC
pc_test
mailx -n -N -f "$(pwd)/in.mbox" <cmds >out 2>err || pc_fail "mailx: nonzero status"
grep BODY-ONE sub/inside.out >/dev/null 2>&1 || pc_fail "cd sub did not make sub the directory for relative names"
grep BODY-TWO home.out >/dev/null 2>&1 || pc_fail "cd with no argument did not go to HOME"
grep BODY-THREE sub/ch.out >/dev/null 2>&1 || pc_fail "chdir sub did not work like cd"
pc_pass
