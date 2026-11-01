# uudecode: a mode that cannot be set is not an error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uudecode.015
# MUTANT    uudecode.mode_error
# MUTANT    uudecode.status1
# MUTANT    uudecode.drop_last
# When the output pathname names an existing file the user can write but
# whose permission bits the user cannot change (another owner), the file is
# overwritten and uudecode succeeds.
. "$PC_CORE/pc.sh"
pc_begin XCU.uudecode.015
LC_ALL=C; export LC_ALL
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
printf 'begin 644 shared.out\n#0V%%T\n`\nend\n' >"$W/in" && chmod 644 "$W/in" || pc_unresolved "setup: cannot write input"
printf Cat >exp || pc_unresolved "setup: cannot write expectation"
# The target: a file another owner made, writable by everyone.
if [ -n "$asu" ]; then
	: >"$W/shared.out" && chmod 666 "$W/shared.out" || pc_unresolved "cannot create the shared file"
	target=$W/shared.out
else
	# An ordinary user: the null device is writable by everyone and not
	# owned by an ordinary user.
	[ -w /dev/null ] && [ ! -O /dev/null ] || pc_unresolved "no writable file of another owner is available"
	target=/dev/null
fi
export target
pc_test
if [ -d pc-mutbin ]; then
	for pcf in pc-mutbin/*; do
		cat "$pcf" >"$area/bin/${pcf##*/}" && chmod 755 "$area/bin/${pcf##*/}" || pc_unresolved "cannot place the fault"
	done
	PATH=$area/bin:$PATH
fi
$asu "${PC_SH:-sh}" -c 'cd "$W" && TMPDIR=$W exec uudecode -o "$target" in' >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "uudecode exited with status $rc although only the mode could not be set"
if [ -n "$asu" ]; then
	cmp -s exp "$target" 2>/dev/null || pc_fail "the writable file was not overwritten"
fi
pc_pass
