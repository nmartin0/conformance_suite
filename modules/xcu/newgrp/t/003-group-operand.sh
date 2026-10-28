# newgrp with the group the process already has.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.newgrp.003
. "$PC_CORE/pc.sh"
pc_begin XCU.newgrp.003
LC_ALL=C; export LC_ALL
d=pcnewgrp.$$
mkdir "$d" || pc_fail "cannot make $d"
trap 'rm -rf "$d"' EXIT
g=$(id -g) && r0=$(id -r -g) || pc_unresolved "id could not report the group IDs"
case $g$r0 in *[!0-9]*|'') pc_unresolved "id gave '$g' and '$r0' for the group IDs" ;; esac
pc_test
printf '%s\n' 'echo "e=$(id -g)"' 'echo "r=$(id -r -g)"' |
	newgrp "$g" >"$d/out" 2>"$d/err"
rc=$?
if [ "$rc" -ne 0 ] && [ ! -s "$d/out" ]; then
	pc_fail "newgrp $g exited with status $rc and the shell did not run"
fi
e=$(sed -n 's/^e=//p' "$d/out")
r=$(sed -n 's/^r=//p' "$d/out")
[ -n "$e" ] && [ -n "$r" ] || pc_fail "the new shell reported no group IDs"
[ "$e" = "$r" ] || [ "$r" = "$r0" ] || pc_fail "real group $r and effective group $e differ in the new shell"
[ "$e" = "$g" ] || pc_fail "the effective group is $e in the new shell, expected $g"
pc_pass
