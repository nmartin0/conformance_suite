# SHELL is called with -c, -- and the command
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mailx.026
# The ! command and the pipe command run the command through the program
# named by SHELL, given exactly three arguments: -c, --, and the command.
. "$PC_CORE/pc.sh"
pc_begin XCU.mailx.026
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
log=$(pwd)/sh.log
{
	printf '#!%s\n' "$shpath"
	printf '{ printf "%%s\\n" "$#" "$1" "$2" "$3"; } >>"%s"\n' "$log"
	printf 'exec "%s" "$@"\n' "$shpath"
} >mysh || pc_unresolved "setup: cannot write the shell wrapper"
chmod +x mysh || pc_unresolved "setup: cannot make the wrapper executable"
SHELL=$(pwd)/mysh; export SHELL
mkcmd '!:' 'pipe 1 cat>/dev/null' >cmds
logs_has() {
	awk -v a="$1" '{ l[NR] = $0 } END { for (i = 1; i + 3 <= NR; i++) if (l[i] == "3" && l[i+1] == "-c" && l[i+2] == "--" && l[i+3] == a) f = 1; exit !f }' sh.log
}
pc_test
mailx -n -N -f in.mbox <cmds >out 2>err || pc_fail "mailx: nonzero status"
[ -f sh.log ] || pc_fail "the program named by SHELL was never run"
logs_has ':' || pc_fail "! did not call SHELL with -c, -- and the command: $(cat sh.log)"
logs_has 'cat>/dev/null' || pc_fail "pipe did not call SHELL with -c, -- and the command: $(cat sh.log)"
pc_pass
