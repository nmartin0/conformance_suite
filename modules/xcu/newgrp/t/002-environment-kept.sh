# The new shell keeps exported variables, directory and umask.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.newgrp.002
. "$PC_CORE/pc.sh"
pc_begin XCU.newgrp.002
LC_ALL=C; export LC_ALL
# Which shell newgrp creates is up to the system; name one with SHELL so that every account gets the same.
SHELL=${PC_SH:-/bin/sh}; export SHELL
d=pcnewgrp.$$
mkdir "$d" || pc_fail "cannot make $d"
trap 'cd ..; rm -rf "$d"' EXIT
cd "$d" || pc_fail "cannot enter $d"
PCNG_VAR=kept_value; export PCNG_VAR
umask 027
want_pwd=$(pwd -P)
want_um=$(umask)
want_um=${want_um#"${want_um%%[!0]*}"}
pc_test
printf '%s\n' 'echo "v=$PCNG_VAR"' 'echo "d=$(pwd -P)"' 'echo "m=$(umask)"' |
	newgrp >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_unresolved "newgrp exited with status $rc before running the commands"
v=$(sed -n 's/^v=//p' out)
p=$(sed -n 's/^d=//p' out)
m=$(sed -n 's/^m=//p' out)
[ -n "$m" ] || pc_unresolved "the new shell did not report the mask"
m=${m#"${m%%[!0]*}"}
[ "$v" = kept_value ] || pc_fail "the exported variable has the value '$v' in the new shell"
[ "$p" = "$want_pwd" ] || pc_fail "the working directory is '$p' in the new shell, expected '$want_pwd'"
[ "$m" = "$want_um" ] || pc_fail "the mask is '$m' in the new shell, expected '$want_um'"
pc_pass
