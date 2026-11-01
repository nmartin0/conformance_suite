# The file type primaries.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.test.007
# MUTANT    test.e_dir_false
# VARIANT   test.argv0_single
# MUTANT    test.e_fifo_false
# MUTANT    test.e_none_true
# MUTANT    test.f_dir_true
# MUTANT    test.f_fifo_true
# MUTANT    test.f_none_true
# MUTANT    test.f_reg_false
# MUTANT    test.d_nondir_true
# MUTANT    test.d_dir_false
# MUTANT    test.d_none_true
# MUTANT    test.L_dangling_false
# MUTANT    test.L_exists_true
# MUTANT    test.L_none_true
# MUTANT    test.h_dangling_false
# MUTANT    test.h_exists_true
# MUTANT    test.p_dir_true
# MUTANT    test.p_reg_true
# MUTANT    test.p_fifo_false
# MUTANT    test.u_exists_true
# MUTANT    test.u_as_g
# MUTANT    test.u_false
# MUTANT    test.g_exists_true
# MUTANT    test.g_as_u
# MUTANT    test.g_false
# MUTANT    test.fn_e_dir_false
# MUTANT    test.te_e_dir_false
# MUTANT    test.br_e_dir_false
. "$PC_CORE/pc.sh"
pc_begin XCU.test.007
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
: >reg || pc_unresolved "setup: cannot create a file"
mkdir dir || pc_unresolved "setup: cannot create a directory"
ln -s reg lnreg || pc_unresolved "setup: cannot create a symbolic link"
ln -s nowhere lnbad || pc_unresolved "setup: cannot create a dangling link"
mkfifo fifo || pc_unresolved "setup: cannot create a FIFO"
: >su || pc_unresolved "setup: cannot create a file"
: >sg || pc_unresolved "setup: cannot create a file"
chmod 4755 su || pc_unresolved "setup: cannot set the set-user-ID flag"
chmod 2755 sg || pc_unresolved "setup: cannot set the set-group-ID flag"
# The flags are checked as set; a process that is not allowed to keep the
# set-group-ID flag on its own file (it is not in the file's group) has
# it cleared by chmod, and then the -g checks are left out.
hasu=no
hasg=no
test -n "$(find su -perm -4000 -print)" && hasu=yes
test -n "$(find sg -perm -2000 -print)" && hasg=yes
[ "$hasu" = yes ] || pc_unresolved "setup: the set-user-ID flag was not kept"
pc_test
chk 0 -e reg
chk 0 -e dir
chk 0 -e fifo
chk 1 -e none
chk 0 -f reg
chk 1 -f dir
chk 1 -f fifo
chk 1 -f none
chk 0 -d dir
chk 1 -d reg
chk 1 -d none
chk 0 -L lnreg
chk 0 -L lnbad
chk 1 -L reg
chk 1 -L dir
chk 1 -L none
chk 0 -h lnreg
chk 0 -h lnbad
chk 1 -h reg
chk 1 -h none
chk 0 -p fifo
chk 1 -p reg
chk 1 -p dir
chk 1 -p none
chk 0 -u su
chk 1 -u sg
chk 1 -u reg
chk 1 -u none
chk 1 -g su
chk 1 -g reg
chk 1 -g none
if [ "$hasg" = yes ]; then
	chk 0 -g sg
fi
pc_pass
