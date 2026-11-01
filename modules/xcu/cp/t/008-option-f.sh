# cp -f replaces a destination file that cannot be opened for writing, by removing it first.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.008
# MUTANT    cp.f_ignored
# MUTANT    cp.f_noop
# MUTANT    cp.f_keeps_old
#
# The destination can be opened by a privileged process anyway, so cp runs
# as a user without privilege (a child drops it when the test runs as root);
# the faults are standalone programs first on PATH, which that child finds.
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.008
nl='
'
slurp() {
	data=
	l=
	while IFS= read -r l; do
		data=$data$l$nl
		l=
	done <"$1"
	[ -z "$l" ] || data=$data$l'<noeol>'
	return 0
}

# The work below is done by a process without privilege: when the test runs
# as root, a child drops it (group and user 65534), because a privileged cp
# can open anything and the verdict would not mean the same as for an
# ordinary user. The private directory of the test cannot be reached by
# another user, so the work happens in a directory of its own under /tmp.
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
$asu "${PC_SH:-sh}" -c 'cd "$W" && umask 022 && printf "old\n" >dst && chmod 400 dst && printf "new\n" >src' || pc_unresolved "cannot create the files"
pc_test
if [ -d pc-mutbin ]; then
	for pcf in pc-mutbin/*; do
		cat "$pcf" >"$area/bin/${pcf##*/}" && chmod 755 "$area/bin/${pcf##*/}" || pc_unresolved "cannot place the fault"
	done
	PATH=$area/bin:$PATH
fi
$asu "${PC_SH:-sh}" -c 'cd "$W" && exec cp -f src dst' >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cp -f exited with status $rc"
slurp "$W/dst"
[ "$data" = "new${nl}" ] || pc_fail "destination was not replaced"
pc_pass
