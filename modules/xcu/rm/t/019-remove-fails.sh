# When a file cannot be removed, rm writes a diagnostic, exits non-zero and goes on to the remaining operands; -f does not hide that.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rm.019
# MUTANT    rm.perm_silent
# MUTANT    rm.perm_status0
# MUTANT    rm.perm_aborts
# MUTANT    rm.perm_diag_stdout
# MUTANT    rm.perm_f_silent
# MUTANT    rm.perm_f_status0
# VARIANT   rm.xref
# VARIANT   rm.xstatus2
# VARIANT   rm.xstatus255
. "$PC_CORE/pc.sh"
pc_begin XCU.rm.019
# The work is done by a process without privilege: when the test runs as
# root, a child drops it (group and user 65534), because a privileged rm
# can remove from any directory. The private directory of the test cannot be
# reached by another user, so the work happens under /tmp.
trap 'chmod -R u+rwx "$area" 2>/dev/null; rm -rf "$area"' EXIT
asu=
if [ "$(id -u)" -eq 0 ]; then
	cat >asu.c <<'PC_EOF'
#include <unistd.h>
int main(int argc, char **argv)
{
	if (argc < 2) return 98;
	if (setgid(65534) != 0 || setuid(65534) != 0 || getuid() == 0) return 99;
	execvp(argv[1], argv + 1);
	return 127;
}
PC_EOF
	${PC_CC:-cc} -D_POSIX_C_SOURCE=202405L -o asu asu.c >/dev/null 2>&1 || pc_unresolved "cannot build the helper that drops privilege"
	asu=$PWD/asu
fi
area=/tmp/pcasu.$$
(umask 022; mkdir "$area" "$area/bin") || pc_unresolved "cannot create $area"
W=$area/w
(umask 022; mkdir "$W") && chmod 777 "$W" || pc_unresolved "cannot create the work directory"
export W
$asu "${PC_SH:-sh}" -c 'cd "$W" && umask 022 && mkdir p && printf x >p/locked && printf x >free1 && printf x >free2 && chmod 555 p' || pc_unresolved "cannot create the tree"
# the directory p does not permit removing what it holds
$asu "${PC_SH:-sh}" -c 'cd "$W" && rm -f p/locked 2>/dev/null; test -f p/locked' || pc_unresolved "the file in p can be removed by an unprivileged process"
pc_test
if [ -d pc-mutbin ]; then
	for pcf in pc-mutbin/*; do
		cat "$pcf" >"$area/bin/${pcf##*/}" && chmod 755 "$area/bin/${pcf##*/}" || pc_unresolved "cannot place the fault"
	done
	PATH=$area/bin:$PATH
fi
$asu "${PC_SH:-sh}" -c 'cd "$W" && exec rm p/locked free1' >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "rm exited 0 although p/locked could not be removed"
[ -s err ] || pc_fail "no diagnostic on standard error for p/locked"
[ ! -s out ] || pc_fail "rm wrote to standard output"
[ -f "$W/p/locked" ] || pc_fail "p/locked is gone"
[ ! -e "$W/free1" ] || pc_fail "the operand after the failing one was not removed"
$asu "${PC_SH:-sh}" -c 'cd "$W" && exec rm -f p/locked free2' >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "rm -f exited 0 although p/locked could not be removed"
[ -s err ] || pc_fail "rm -f gave no diagnostic for p/locked"
[ -f "$W/p/locked" ] || pc_fail "p/locked is gone after rm -f"
[ ! -e "$W/free2" ] || pc_fail "rm -f did not remove the operand after the failing one"
pc_pass
