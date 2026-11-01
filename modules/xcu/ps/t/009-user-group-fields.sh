# user, ruser, group, rgroup fields.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ps.009
# MUTANT    ps.user_wrong
# MUTANT    ps.ruser_wrong
# MUTANT    ps.group_wrong
# MUTANT    ps.rgroup_wrong
# MUTANT    ps.user_is_real
# MUTANT    ps.ruser_is_eff
# MUTANT    ps.group_is_real
# MUTANT    ps.rgroup_is_eff
# MUTANT    ps.ids_empty
# VARIANT   ps.ids_numeric
# VARIANT   ps.padded
. "$PC_CORE/pc.sh"
pc_begin XCU.ps.009
LC_ALL=C; export LC_ALL
fp=
trap '[ -n "$fp" ] && { kill "$fp" 2>/dev/null; wait "$fp" 2>/dev/null; }' EXIT
# When the suite runs with privilege, a process whose effective user and group
# differ from its real ones exists too (effective 65534, real the invoker's),
# so that the effective and the real fields can be told apart.
if [ "$(id -u)" -eq 0 ]; then
	cat >asx.c <<'PC_EOF'
#include <unistd.h>
int main(int argc, char **argv)
{
	if (argc < 2) return 98;
	if (setegid(65534) != 0 || seteuid(65534) != 0 || geteuid() == getuid()) return 99;
	execvp(argv[1], argv + 1);
	return 127;
}
PC_EOF
	if ${PC_CC:-cc} -D_POSIX_C_SOURCE=202405L -o asx asx.c >/dev/null 2>&1; then
		./asx sleep 303 </dev/null >/dev/null 2>&1 &
		fp=$!
	fi
fi
# chk FIELD NAME-OPTION ID-OPTION
chk() {
	ps -o "$1=" -p "$$" >out 2>err </dev/null || pc_fail "ps -o $1= failed"
	got=
	read -r got <out
	[ -n "$got" ] || pc_fail "ps -o $1= wrote nothing for process $$"
	nm=$(id "$2" 2>/dev/null)
	num=$(id "$3") || pc_fail "id $3 failed"
	[ -n "$num" ] || pc_fail "id $3 wrote nothing"
	if [ -n "$nm" ] && [ "$got" = "$nm" ]; then return 0; fi
	[ "$got" = "$num" ] && return 0
	pc_fail "$1 is '$got', expected '$nm' or '$num'"
}
# val FIELD PID: g = the value of the field for the process
val() {
	ps -o "$1=" -p "$2" >out 2>err </dev/null || pc_fail "ps -o $1= -p $2 failed"
	g=
	read -r g <out
	[ -n "$g" ] || pc_fail "ps -o $1= wrote nothing for process $2"
}
# realdiff REALFIELD EFFFIELD REALNAMEOPT REALIDOPT: for the process with
# differing identities, the real field is the invoker's and the effective one
# is not.
realdiff() {
	val "$1" "$fp"
	r=$g
	nm=$(id "$3" 2>/dev/null)
	num=$(id "$4") || pc_fail "id $4 failed"
	{ [ -n "$nm" ] && [ "$r" = "$nm" ]; } || [ "$r" = "$num" ] || pc_fail "$1 of $fp is '$r', expected '$nm' or '$num'"
	val "$2" "$fp"
	[ "$g" != "$nm" ] && [ "$g" != "$num" ] || pc_fail "$2 of $fp is '$g', the real one, not the effective one"
}
# wait until the other process is that user's sleep (setup phase: ps is the real one)
if [ -n "$fp" ]; then
	i=0
	while :; do
		c=$(ps -o comm= -p "$fp" 2>/dev/null </dev/null)
		[ "$c" = sleep ] && break
		i=$((i + 1))
		[ "$i" -lt 10 ] || { fp=; break; }
		sleep 1
	done
fi
pc_test
chk user -un -u
chk ruser -urn -ur
chk group -gn -g
chk rgroup -grn -gr
if [ -n "$fp" ]; then
	realdiff ruser user -urn -ur
	realdiff rgroup group -grn -gr
fi
pc_pass
