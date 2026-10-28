# edit and visual
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.032
# edit and visual put each named message in a temporary file and run the
# program named by EDITOR or VISUAL on it in turn, without changing the
# messages in the mailbox.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.032
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
shpath=$(command -v sh) || pc_unresolved "setup: no sh found"
case $shpath in /*) ;; *) pc_unresolved "setup: no absolute path for sh" ;; esac
mkwrap() { # wrapper name, log file
	{
		printf '#!%s\n' "$shpath"
		printf 'for pc_last; do :; done\n'
		printf '{ printf "BEGIN\\n"; cat "$pc_last"; printf "END\\n"; } >>"%s"\n' "$2"
		printf 'printf "MODIFIED-TEXT\\n" >"$pc_last"\n'
	} >"$1" && chmod +x "$1"
}
mkwrap ed1 "$(pwd)/ed1.log" || pc_unresolved "setup: cannot write the editor wrapper"
mkwrap ed2 "$(pwd)/ed2.log" || pc_unresolved "setup: cannot write the editor wrapper"
EDITOR=$(pwd)/ed1; VISUAL=$(pwd)/ed2; export EDITOR VISUAL
mkcmd 'edit 1 2' 'visual 3' 'q' >cmds
pc_test
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
[ -f ed1.log ] || pc_fail "edit did not run the program named by EDITOR"
[ -f ed2.log ] || pc_fail "visual did not run the program named by VISUAL"
grep BODY-ONE ed1.log >/dev/null || pc_fail "edit 1 2: message 1 was not in the temporary file"
grep BODY-TWO ed1.log >/dev/null || pc_fail "edit 1 2: message 2 was not in the temporary file"
grep BODY-THREE ed1.log >/dev/null && pc_fail "edit 1 2 gave message 3 to the editor"
l1=$(grep -n BODY-ONE ed1.log | sed -n 1p | cut -d: -f1)
l2=$(grep -n BODY-TWO ed1.log | sed -n 1p | cut -d: -f1)
[ "$l1" -lt "$l2" ] || pc_fail "the messages were not edited in order"
grep BODY-THREE ed2.log >/dev/null || pc_fail "visual 3: message 3 was not in the temporary file"
grep -e BODY-ONE -e BODY-TWO ed2.log >/dev/null && pc_fail "visual 3 gave other messages to the editor"
grep MODIFIED-TEXT in.mbox >/dev/null && pc_fail "edit or visual changed a message in the mailbox"
[ "$(grep -c '^From ' in.mbox)" -eq 3 ] || pc_fail "the mailbox no longer holds three messages"
for t in ONE TWO THREE; do grep BODY-$t in.mbox >/dev/null || pc_fail "message $t was lost from the mailbox"; done
pc_pass
