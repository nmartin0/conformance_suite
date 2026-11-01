# grep exits 0 when a line is selected, 1 when none is, above 1 on error; -s hides messages for unreadable files and -q gives 0 despite an error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.grep.008
# MUTANT    grep.status_match_1
# MUTANT    grep.status_nomatch_0
# MUTANT    grep.status_nomatch_2
# MUTANT    grep.err_status1
# MUTANT    grep.err_status0
# MUTANT    grep.err_no_diag
# MUTANT    grep.err_stdout
# MUTANT    grep.err_stops
# MUTANT    grep.s_ignored
# MUTANT    grep.s_status0
# MUTANT    grep.s_hides_all
# MUTANT    grep.s_only_missing
# MUTANT    grep.q_error_status
# MUTANT    grep.q_stops
# VARIANT   grep.status3
# VARIANT   grep.status255
# VARIANT   grep.error_status_wins
. "$PC_CORE/pc.sh"
pc_begin XCU.grep.008
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a\n' >f1 || pc_fail "cannot create input"
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"

# The permission-denied case is run by a process without privilege: when the
# test runs as root, a child drops it (group and user 65534), because a
# privileged grep can read any file. The work happens in a directory of its
# own under /tmp, which that user can reach.
trap 'rm -rf "$area"' EXIT
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
printf 'a\n' >"$W/unr" && printf 'a\n' >"$W/ok" || pc_unresolved "cannot create the files"
chmod 000 "$W/unr" || pc_unresolved "cannot change the mode"
$asu "${PC_SH:-sh}" -c 'cat "$W/unr"' >/dev/null 2>&1 </dev/null && pc_unresolved "the unprivileged process can read a mode 000 file"
pc_test
if [ -d pc-mutbin ]; then
	for pcf in pc-mutbin/*; do
		cat "$pcf" >"$area/bin/${pcf##*/}" && chmod 755 "$area/bin/${pcf##*/}" || pc_unresolved "cannot place the fault"
	done
	PATH=$area/bin:$PATH
fi

grep a f1 >/dev/null 2>&1 </dev/null
[ $? -eq 0 ] || pc_fail "status was not 0 for a match"
grep b f1 >/dev/null 2>&1 </dev/null
[ $? -eq 1 ] || pc_fail "status was not 1 for no match"
grep a nosuchfile >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -gt 1 ] || pc_fail "status was not above 1 for a missing file"
err=$(grep a nosuchfile 2>&1 >/dev/null </dev/null)
[ -n "$err" ] || pc_fail "no diagnostic for a missing file"
err=$(grep -s a nosuchfile 2>&1 >/dev/null </dev/null)
[ -z "$err" ] || pc_fail "-s did not suppress the missing-file diagnostic"
grep -s a nosuchfile >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -gt 1 ] || pc_fail "-s changed the failure status"
grep -q a nosuchfile f1 >/dev/null 2>&1 </dev/null
[ $? -eq 0 ] || pc_fail "-q did not give status 0 after a match despite an earlier error"
# processing goes on with the next operand after one that cannot be read
got=$(grep a nosuchfile f1 2>/dev/null </dev/null)
[ "$got" = "f1:a" ] || pc_fail "the file after a missing one was not searched"
# -s does not suppress other diagnostics (here: no pattern at all)
err=$(grep -s 2>&1 >/dev/null </dev/null)
[ -n "$err" ] || pc_fail "-s suppressed a diagnostic that is not about a file"

# an existing file that cannot be read, as an unprivileged process
err=$($asu "${PC_SH:-sh}" -c 'cd "$W" && exec grep a unr' 2>&1 >/dev/null </dev/null)
rc=$?
[ "$rc" -gt 1 ] || pc_fail "status was not above 1 for an unreadable file (status $rc)"
[ -n "$err" ] || pc_fail "no diagnostic for an unreadable file"
err=$($asu "${PC_SH:-sh}" -c 'cd "$W" && exec grep -s a unr' 2>&1 >/dev/null </dev/null)
[ -z "$err" ] || pc_fail "-s did not suppress the unreadable-file diagnostic"
$asu "${PC_SH:-sh}" -c 'cd "$W" && exec grep -s a unr' >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -gt 1 ] || pc_fail "-s changed the status for an unreadable file (status $rc)"
got=$($asu "${PC_SH:-sh}" -c 'cd "$W" && exec grep a unr ok' 2>/dev/null </dev/null)
[ "$got" = "ok:a" ] || pc_fail "the file after an unreadable one was not searched"
pc_pass
