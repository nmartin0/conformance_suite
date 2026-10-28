# echo, comments, quoting, source, ! and set
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.009
# echo writes its strings as the echo utility does (abbreviations accepted,
# quotes keep spacing), a # line is ignored, source runs commands from a
# file, ! runs a shell command, and set defines a variable that set lists
# and unset removes.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.009
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
cat >srcfile <<'EOC'
echo from-source
EOC
cat >cmds <<'EOC'
set noprompt
# a comment line
echo @@a@@
ec one
ech two
echo three
echo @@b@@
echo x   y
echo "p  q"
echo @@c@@
source srcfile
echo @@d@@
!echo ran >bang.out
set myvar=hello
set
unset myvar
echo @@e@@
set
EOC
pc_test
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
sect a | grep -x one >/dev/null || pc_fail "ec did not echo: $(sect a)"
sect a | grep -x two >/dev/null || pc_fail "ech did not echo"
sect a | grep -x three >/dev/null || pc_fail "echo did not echo"
sect b | grep -Fx 'x y' >/dev/null || pc_fail "unquoted echo arguments not joined by one space: $(sect b)"
sect b | grep -Fx 'p  q' >/dev/null || pc_fail "quotes did not keep the spacing: $(sect b)"
has c from-source || pc_fail "source did not run the commands in the file: $(sect c)"
[ "$(cat bang.out 2>/dev/null)" = ran ] || pc_fail "! did not run the shell command"
sect d | grep myvar >/dev/null || pc_fail "set does not list myvar: $(sect d)"
sect d | grep hello >/dev/null || pc_fail "set does not list the value of myvar"
has e myvar && pc_fail "unset did not remove myvar"
pc_pass
