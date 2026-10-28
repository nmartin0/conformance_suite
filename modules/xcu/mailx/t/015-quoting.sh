# argument quoting and line continuation
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.015
# Command arguments can be enclosed in paired double or single quotes that
# keep white space and shell characters literally, the other kind of quote
# is literal inside, a backslash outside quotes is dropped and the next
# character is taken literally, and a backslash at the end of a line joins
# the next line to the command.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.015
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
echo @@a@@
echo "a  b"
echo 'c  d'
echo "it's"
echo 'say "hi"'
echo @@b@@
echo x\ \ y
echo p\"q
echo @@c@@
echo one\
two
echo '$HOME'
EOC
pc_test
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
sect a | grep -Fx 'a  b' >/dev/null || pc_fail "double quotes did not keep the spacing: $(sect a)"
sect a | grep -Fx 'c  d' >/dev/null || pc_fail "single quotes did not keep the spacing: $(sect a)"
sect a | grep -Fx "it's" >/dev/null || pc_fail "a single quote inside double quotes was not literal: $(sect a)"
sect a | grep -Fx 'say "hi"' >/dev/null || pc_fail "double quotes inside single quotes were not literal: $(sect a)"
sect b | grep -Fx 'x  y' >/dev/null || pc_fail "backslash-blank did not keep the blanks in one argument: $(sect b)"
sect b | grep -Fx 'p"q' >/dev/null || pc_fail "backslash-quote was not taken literally: $(sect b)"
sect c | grep -Fx 'onetwo' >/dev/null || pc_fail "a trailing backslash did not join the next line: $(sect c)"
sect c | grep -Fx '$HOME' >/dev/null || pc_fail "single quotes did not keep a dollar sign literal: $(sect c)"
pc_pass
