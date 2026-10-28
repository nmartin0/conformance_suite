# Creating a file where one of an incompatible type exists fails with a diagnostic and leaves the existing object as it was: tee onto a directory; mkdir over a directory, file or symbolic link; mkfifo over a file, FIFO, directory or symbolic link; ln -s over a file, symbolic link or FIFO
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.1-relationship.005
. "$PC_CORE/pc.sh"
pc_begin XCU.1.1-relationship.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mkdir d || pc_unresolved "mkdir"
printf 'x\n' >f || pc_unresolved "write"
mkfifo p || pc_unresolved "mkfifo"
sl=0
ln -s tgt l 2>/dev/null && [ -h l ] && sl=1
pc_test
chk() {
	label=$1; shift
	"$@" >o 2>e </dev/null
	rc=$?
	[ "$rc" -ne 0 ] || pc_fail "$label: exit status 0"
	[ -s e ] || pc_fail "$label: no diagnostic"
}
chk "tee onto a directory" tee d
[ -d d ] || pc_fail "the directory is gone"
chk "mkdir over a directory" mkdir d
chk "mkdir over a file" mkdir f
[ -f f ] && [ "$(cat f)" = x ] || pc_fail "the file changed after mkdir"
chk "mkfifo over a file" mkfifo f
[ -f f ] || pc_fail "the file is gone after mkfifo"
chk "mkfifo over a FIFO" mkfifo p
[ -p p ] || pc_fail "the FIFO is gone"
chk "mkfifo over a directory" mkfifo d
[ -d d ] || pc_fail "the directory is gone after mkfifo"
chk "ln -s over a file" ln -s other f
[ -f f ] || pc_fail "the file is gone after ln -s"
chk "ln -s over a FIFO" ln -s other p
[ -p p ] || pc_fail "the FIFO is gone after ln -s"
if [ "$sl" = 1 ]; then
	chk "mkdir over a symbolic link" mkdir l
	chk "mkfifo over a symbolic link" mkfifo l
	chk "ln -s over a symbolic link" ln -s other l
	[ -h l ] && ls -l l | grep 'tgt$' >/dev/null || pc_fail "the symbolic link changed"
fi
pc_pass
