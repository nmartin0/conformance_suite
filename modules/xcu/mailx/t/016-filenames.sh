# file name transformations
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.016
# A file name starting with an unquoted + is placed in the folder directory
# (a relative folder is taken from HOME, an absolute one is used as it is),
# stays unchanged when folder is unset, shell word expansion applies to
# file names, and quoting keeps characters literal.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.016
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
mkdir fld fld2 || pc_unresolved "setup: cannot make directories"
ABS=$(pwd)
cat >cmds <<EOC
set noprompt
set folder=fld
save 1 +rel
set folder="$ABS/fld2"
save 1 +abs
unset folder
save 1 +lit
save 1 \$HOME/exp.out
save 1 '\$Q.out'
set folder=fld
save 1 '+qq'
EOC
pc_test
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
grep BODY-ONE fld/rel >/dev/null 2>&1 || pc_fail "+rel with a relative folder did not go to HOME/folder/rel"
grep BODY-ONE fld2/abs >/dev/null 2>&1 || pc_fail "+abs with an absolute folder did not go to folder/abs"
grep BODY-ONE ./+lit >/dev/null 2>&1 || pc_fail "with folder unset, +lit was not the file named +lit"
grep BODY-ONE exp.out >/dev/null 2>&1 || pc_fail "\$HOME in a file name was not expanded"
grep BODY-ONE './$Q.out' >/dev/null 2>&1 || pc_fail "a quoted dollar sign in a file name was not literal"
grep BODY-ONE ./+qq >/dev/null 2>&1 || pc_fail "a quoted + in a file name was treated as the folder prefix"
pc_pass
