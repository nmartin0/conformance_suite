# errors and invalid commands in the start-up file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.047
# An error in the start-up file (an unknown command, or a command that is not
# valid there such as !) either ends mailx with a diagnostic and a non-zero
# status, or lets it continue after a diagnostic but ignoring the rest of
# the file; the invalid command is not carried out.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.047
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
printf 'set noprompt\nxyzzy-no-such-command\nset pcv_after=1\n' >rc1 || pc_unresolved "setup: cannot write a start-up file"
printf 'set noprompt\n!echo ran >bang.rc.out\nset pcv_after=1\n' >rc2 || pc_unresolved "setup: cannot write a start-up file"
printf 'echo @@a@@\nset\n' >cmds
pc_test
for n in 1 2; do
	MAILRC=$(pwd)/rc$n; export MAILRC
	mailx -n -N -f in.mbox <cmds >out 2>err; st=$?
	[ -s err ] || pc_fail "start-up file $n: no diagnostic was written to standard error"
	if [ "$st" -eq 0 ]; then
		has a pcv_after && pc_fail "start-up file $n: mailx went on after the error and read the rest of the file"
	fi
done
[ -e bang.rc.out ] && pc_fail "the ! command in the start-up file was carried out"
pc_pass
