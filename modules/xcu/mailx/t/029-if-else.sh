# if, else and endif
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.029
# if r runs the following commands in Receive Mode, if s does not, else
# reverses that, and endif ends the block.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.029
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
if r
echo IFR-YES
else
echo IFR-NO
endif
if s
echo IFS-YES
else
echo IFS-NO
endif
if r
echo IFR2-YES
endif
if s
echo IFS2-YES
endif
if s
echo IFS3-YES
el
echo IFS3-NO
en
EOC
pc_test
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
for l in IFR-YES IFS-NO IFR2-YES IFS3-NO; do grep -Fx $l out >/dev/null || pc_fail "$l was not written"; done
for l in IFR-NO IFS-YES IFS2-YES IFS3-YES; do grep -Fx $l out >/dev/null && pc_fail "$l was written but should not be"; done
pc_pass
