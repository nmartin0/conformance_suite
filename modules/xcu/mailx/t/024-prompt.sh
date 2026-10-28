# the prompt variable
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.024
# The command mode prompt is written to standard output and is by default
# the string "? "; prompt=string changes it, and a null prompt or noprompt
# turns it off.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.024
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
echo A-ONE
set prompt=PQRS>
echo B-TWO
set prompt=
echo C-THREE
echo D-FOUR
set prompt=PQRS>
set noprompt
echo E-FIVE
echo F-SIX
EOC
pc_test
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
grep -Fx '? A-ONE' out >/dev/null || pc_fail "the default prompt \"? \" was not written before the first command"
grep -F 'PQRS>B-TWO' out >/dev/null || pc_fail "prompt=PQRS> was not used"
grep -F 'PQRS>C-THREE' out >/dev/null || pc_fail "the prompt in force before set prompt= was not written"
grep -Fx 'D-FOUR' out >/dev/null || pc_fail "a null prompt was still written"
grep -Fx 'F-SIX' out >/dev/null || pc_fail "noprompt did not turn the prompt off"
pc_pass
