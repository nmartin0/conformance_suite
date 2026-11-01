# cp -R makes copied directories usable while it fills them and leaves them with the source permissions (modified by the mask): a read-only directory with content is copied with its mode.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.013
# MUTANT    cp.R_dir_status1
# MUTANT    cp.R_dir_not_usable
# MUTANT    cp.R_dir_mode_default
# MUTANT    cp.R_dir_left_rwx
# MUTANT    cp.R_dir_ignores_umask
# MUTANT    cp.R_ro_skips_contents
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.013
# The faults are standalone programs first on PATH, which the unprivileged
# child finds.
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
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
$asu "${PC_SH:-sh}" -c 'cd "$W" && umask 022 && mkdir src && printf "x\n" >src/f && chmod 570 src' || pc_unresolved "cannot create the tree"
pc_test
if [ -d pc-mutbin ]; then
	for pcf in pc-mutbin/*; do
		cat "$pcf" >"$area/bin/${pcf##*/}" && chmod 755 "$area/bin/${pcf##*/}" || pc_unresolved "cannot place the fault"
	done
	PATH=$area/bin:$PATH
fi
$asu "${PC_SH:-sh}" -c 'cd "$W" && umask 022 && exec cp -R src dst' >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cp -R exited with status $rc"
[ -f "$W/dst/f" ] || pc_fail "dst/f was not copied"
modeof "$W/dst" || pc_fail "cannot list dst"
[ "$mo" = "dr-xr-x---" ] || pc_fail "dst has mode $mo, expected dr-xr-x---"
pc_pass
