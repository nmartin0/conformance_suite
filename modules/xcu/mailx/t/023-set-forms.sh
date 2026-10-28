# set and unset forms
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.023
# set name, set name= and set name=string define variables that set lists,
# set noname erases one like unset, and unset takes several names.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.023
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
set pcv_a=1 pcv_b=two pcv_c= pcv_d
echo @@a@@
set
set nopcv_b
echo @@b@@
set
unset pcv_a pcv_c
echo @@c@@
set
set pcv_q="x y"
echo @@d@@
set
EOC
pc_test
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
for v in pcv_a pcv_b pcv_c pcv_d; do has a $v || pc_fail "set does not list $v: $(sect a)"; done
has a two || pc_fail "set does not list the value of pcv_b"
has b pcv_b && pc_fail "set nopcv_b did not erase pcv_b"
has b pcv_a || pc_fail "set nopcv_b erased pcv_a as well"
has c pcv_a && pc_fail "unset did not erase pcv_a"
has c pcv_c && pc_fail "unset did not erase pcv_c"
has c pcv_d || pc_fail "unset of two names erased pcv_d too"
has d 'x y' || pc_fail "a quoted value with a blank is not listed whole: $(sect d)"
pc_pass
