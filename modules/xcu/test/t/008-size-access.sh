# -s, -r, -w and -x.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.test.008
# MUTANT    test.s_empty_true
# MUTANT    test.s_full_false
# MUTANT    test.s_none_true
# MUTANT    test.r_missing_true
# MUTANT    test.w_missing_true
# MUTANT    test.x_missing_true
# MUTANT    test.r_denied_true
# MUTANT    test.w_denied_true
# MUTANT    test.x_denied_true
# MUTANT    test.r_granted_false
# MUTANT    test.w_granted_false
# MUTANT    test.x_granted_false
# MUTANT    test.x_dir_false
# MUTANT    test.r_dir_false
# MUTANT    test.w_dir_false
# MUTANT    test.r_as_w
# MUTANT    test.w_as_x
# MUTANT    test.x_as_r
# MUTANT    test.fn_s_empty_true
# MUTANT    test.te_s_empty_true
# MUTANT    test.br_s_empty_true
. "$PC_CORE/pc.sh"
pc_begin XCU.test.008
# chk WANT ARGS...: the expression ARGS must give status WANT (0 or 1)
# for test and for [ (with its closing bracket), each called through the
# shell and by a direct exec.
chk() {
	want=$1
	shift
	test "$@" </dev/null >/dev/null 2>&1
	rc=$?
	[ "$rc" -eq "$want" ] || pc_fail "test $* gave status $rc, wanted $want"
	[ "$@" ] </dev/null >/dev/null 2>&1
	rc=$?
	[ "$rc" -eq "$want" ] || pc_fail "[ $* ] gave status $rc, wanted $want"
	(exec test "$@") </dev/null >/dev/null 2>&1
	rc=$?
	[ "$rc" -eq "$want" ] || pc_fail "exec test $* gave status $rc, wanted $want"
	(exec [ "$@" ]) </dev/null >/dev/null 2>&1
	rc=$?
	[ "$rc" -eq "$want" ] || pc_fail "exec [ $* ] gave status $rc, wanted $want"
}
printf 'x\n' >full || pc_unresolved "setup: cannot create a file"
: >empty || pc_unresolved "setup: cannot create a file"
: >rw || pc_unresolved "setup: cannot create a file"
: >prog || pc_unresolved "setup: cannot create a file"
mkdir dir || pc_unresolved "setup: cannot create a directory"
chmod 644 rw || pc_unresolved "setup: chmod failed"
chmod 755 prog || pc_unresolved "setup: chmod failed"
chmod 755 dir || pc_unresolved "setup: chmod failed"
# Denied permission needs a process without privilege: when the test runs
# as root, a child drops it (group and user 65534), as a privileged process
# is granted access whatever the mode. The files below have the same class
# bits in all three classes, so the owner, a group member and anyone else
# are told the same. The child also loads the planted fault, if any, from
# a copy that it can reach.
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
	${PC_CC:-cc} -D_POSIX_C_SOURCE=202405L -o asu asu.c >/dev/null 2>&1 || pc_unresolved "setup: cannot build the helper that drops privilege"
	asu=$PWD/asu
fi
area=/tmp/pcasu.$$
trap 'chmod -R u+rwx "$area" 2>/dev/null; rm -rf "$area"' EXIT
(umask 022; mkdir "$area" "$area/m" "$area/w") || pc_unresolved "setup: cannot create $area"
chmod 777 "$area/w" || pc_unresolved "setup: chmod failed"
for pcm in 444 222 111 000; do
	: >"$area/f$pcm" && mkdir "$area/d$pcm" || pc_unresolved "setup: cannot create the files"
	chmod "$pcm" "$area/f$pcm" "$area/d$pcm" || pc_unresolved "setup: chmod failed"
done
cat >"$area/child.sh" <<'PC_EOF'
cd "$A/w" || exit 97
if [ -n "${PC_MUTANT_FILE:-}" ]; then . "$PC_MUTANT_FILE"; fi
chk() {
	want=$1
	shift
	test "$@" </dev/null >/dev/null 2>&1
	rc=$?
	case $rc in "$want") ;; *) echo "test $* gave status $rc, wanted $want" ;; esac
	[ "$@" ] </dev/null >/dev/null 2>&1
	rc=$?
	case $rc in "$want") ;; *) echo "[ $* ] gave status $rc, wanted $want" ;; esac
	(exec test "$@") </dev/null >/dev/null 2>&1
	rc=$?
	case $rc in "$want") ;; *) echo "exec test $* gave status $rc, wanted $want" ;; esac
	(exec [ "$@" ]) </dev/null >/dev/null 2>&1
	rc=$?
	case $rc in "$want") ;; *) echo "exec [ $* ] gave status $rc, wanted $want" ;; esac
}
chk 0 -r "$A/f444"
chk 1 -w "$A/f444"
chk 1 -x "$A/f444"
chk 1 -r "$A/f222"
chk 0 -w "$A/f222"
chk 1 -x "$A/f222"
chk 1 -r "$A/f111"
chk 1 -w "$A/f111"
chk 0 -x "$A/f111"
chk 1 -r "$A/f000"
chk 1 -w "$A/f000"
chk 1 -x "$A/f000"
chk 0 -r "$A/d444"
chk 1 -w "$A/d444"
chk 1 -x "$A/d444"
chk 1 -r "$A/d222"
chk 0 -w "$A/d222"
chk 1 -x "$A/d222"
chk 1 -r "$A/d111"
chk 1 -w "$A/d111"
chk 0 -x "$A/d111"
chk 1 -r "$A/d000"
chk 1 -w "$A/d000"
chk 1 -x "$A/d000"
echo done
PC_EOF
pc_test
if [ -n "${PC_MUTANT_FILE:-}" ]; then
	cp "${PC_MUTANT_FILE%/*}"/* "$area/m/" && chmod -R a+rX "$area/m" || pc_unresolved "cannot copy the planted fault"
	PC_MUTANT_FILE=$area/m/${PC_MUTANT_FILE##*/}
	export PC_MUTANT_FILE
fi
A=$area
export A
$asu "${PC_SH:-sh}" "$area/child.sh" >child.out 2>&1 </dev/null
pcc_last=
while read -r pcc_line; do
	case $pcc_line in
	done) pcc_last=done ;;
	*) pc_fail "without privilege: $pcc_line" ;;
	esac
done <child.out
[ "$pcc_last" = done ] || pc_fail "the checks without privilege did not finish"
chk 0 -s full
chk 1 -s empty
chk 1 -s none
chk 0 -r rw
chk 1 -r none
chk 0 -w rw
chk 1 -w none
chk 0 -x prog
chk 1 -x rw
chk 1 -x none
chk 0 -x dir
chk 0 -r dir
chk 0 -w dir
pc_pass
