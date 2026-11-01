# When a file cannot be read cmp exits with a status above 1, writes a diagnostic (unless -s), and writes nothing to standard output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cmp.007
# MUTANT    cmp.err_status0
# MUTANT    cmp.err_status1
# MUTANT    cmp.err_no_diag
# MUTANT    cmp.err_stdout
# MUTANT    cmp.err_out_extra
# MUTANT    cmp.err_f1_empty
# MUTANT    cmp.err_f2_empty
# MUTANT    cmp.s_err_status1
# MUTANT    cmp.l_err_status1
# MUTANT    cmp.err_perm_as_empty
# VARIANT   cmp.err_status3
# VARIANT   cmp.s_err_diag
. "$PC_CORE/pc.sh"
pc_begin XCU.cmp.007
printf 'abc\n' >f1 || pc_fail "cannot create f1"
: >e1 || pc_fail "cannot create e1"
[ ! -e nonexistent ] || pc_unresolved "nonexistent exists"

# The permission-denied case is run by a process without privilege: when the
# test runs as root, a child drops it (group and user 65534), because a
# privileged cmp can read any file. The work happens in a directory of its
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
printf 'abc\n' >"$W/unr" && printf 'abc\n' >"$W/ok" || pc_unresolved "cannot create the files"
chmod 000 "$W/unr" || pc_unresolved "cannot change the mode"
$asu "${PC_SH:-sh}" -c 'cat "$W/unr"' >/dev/null 2>&1 </dev/null && pc_unresolved "the unprivileged process can read a mode 000 file"
pc_test
if [ -d pc-mutbin ]; then
	for pcf in pc-mutbin/*; do
		cat "$pcf" >"$area/bin/${pcf##*/}" && chmod 755 "$area/bin/${pcf##*/}" || pc_unresolved "cannot place the fault"
	done
	PATH=$area/bin:$PATH
fi

# chkerr RC OUT ERR WHAT: status above 1, nothing on standard output and,
# unless the diagnostic is optional (-s), a diagnostic.
chkerr() {
	[ "$1" -gt 1 ] || pc_fail "cmp exited with status $1 for $4"
	[ ! -s "$2" ] || pc_fail "output written for $4"
	[ "$5" = optional ] || [ -s "$3" ] || pc_fail "no diagnostic for $4"
}
cmp f1 nonexistent >out 2>err </dev/null
chkerr $? out err "a missing second file"
cmp nonexistent f1 >out 2>err </dev/null
chkerr $? out err "a missing first file"
cmp nonexistent nonexistent >out 2>err </dev/null
chkerr $? out err "two missing files"
cmp e1 nonexistent >out 2>err </dev/null
chkerr $? out err "an empty file and a missing one"
cmp -l f1 nonexistent >out 2>err </dev/null
chkerr $? out err "a missing file with -l"
cmp -l nonexistent f1 >out 2>err </dev/null
chkerr $? out err "a missing first file with -l"
cmp -s f1 nonexistent >out 2>err </dev/null
chkerr $? out err "a missing second file with -s" optional
cmp -s nonexistent f1 >out 2>err </dev/null
chkerr $? out err "a missing first file with -s" optional
# a file that exists but may not be read
$asu cmp "$W/ok" "$W/unr" >out 2>err </dev/null
chkerr $? out err "an unreadable second file"
$asu cmp "$W/unr" "$W/ok" >out 2>err </dev/null
chkerr $? out err "an unreadable first file"
$asu cmp -s "$W/unr" "$W/ok" >out 2>err </dev/null
chkerr $? out err "an unreadable file with -s" optional
# the same operands without the fault are told apart from a failure
$asu cmp "$W/ok" "$W/ok" >out 2>err </dev/null
[ $? -eq 0 ] || pc_fail "cmp failed for a readable file compared with itself"
pc_pass
