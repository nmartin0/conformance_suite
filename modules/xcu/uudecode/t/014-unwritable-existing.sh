# uudecode fails on an existing output file the user cannot write
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uudecode.014
# MUTANT    uudecode.ro_overwrite
# MUTANT    uudecode.ro_replace
# MUTANT    uudecode.ro_status0
# MUTANT    uudecode.ro_nodiag
# VARIANT   uudecode.status2
# VARIANT   uudecode.status255
# If the output pathname names an existing file on which the user has no
# write permission, uudecode ends with an error (nonzero status, a
# diagnostic on standard error) and the file keeps its contents.
. "$PC_CORE/pc.sh"
pc_begin XCU.uudecode.014
LC_ALL=C; export LC_ALL
# The work is done by a process without privilege: when the test runs as
# root, a child drops it (group and user 65534), because a privileged
# uudecode can write anything. The fault programs are standalone programs
# first on PATH, which the unprivileged child finds.
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
printf 'begin 644 ro.out\n#0V%%T\n`\nend\n' >"$W/in" && chmod 644 "$W/in" || pc_unresolved "setup: cannot write input"
printf old >exp || pc_unresolved "setup: cannot write expectation"
$asu "${PC_SH:-sh}" -c 'cd "$W" && umask 022 && printf old >ro.out && chmod 444 ro.out' || pc_unresolved "cannot create the read-only file"
pc_test
if [ -d pc-mutbin ]; then
	for pcf in pc-mutbin/*; do
		cat "$pcf" >"$area/bin/${pcf##*/}" && chmod 755 "$area/bin/${pcf##*/}" || pc_unresolved "cannot place the fault"
	done
	PATH=$area/bin:$PATH
fi
$asu "${PC_SH:-sh}" -c 'cd "$W" && TMPDIR=$W exec uudecode in' >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "uudecode exited with status 0 on an unwritable output file"
[ -s err ] || pc_fail "no diagnostic message was written to standard error"
cmp -s exp "$W/ro.out" 2>/dev/null || pc_fail "the unwritable file was changed"
pc_pass
