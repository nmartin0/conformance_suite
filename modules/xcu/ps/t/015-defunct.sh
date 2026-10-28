# ps marks an unwaited, exited child as defunct
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ps.015
. "$PC_CORE/pc.sh"
pc_begin XCU.ps.015
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
par=
trap '[ -n "$par" ] && { kill "$par" 2>/dev/null; wait "$par" 2>/dev/null; }; cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# the parent starts a child and then replaces itself with a program that never waits
sh -c 'sh -c "echo \$\$ >zpid; exit 0" </dev/null >/dev/null 2>&1 & exec sleep 60' </dev/null >/dev/null 2>&1 &
par=$!
i=0
while [ ! -s zpid ] && [ "$i" -lt 20 ]; do sleep 1; i=$((i+1)); done
z=$(cat zpid 2>/dev/null)
[ -n "$z" ] || pc_unresolved "the child process did not start"
pc_test
i=0
ok=
while [ "$i" -lt 10 ]; do
	ps -p "$z" >out 2>err </dev/null
	if sed 1d out | grep -i 'defunct' >/dev/null 2>&1; then ok=1; break; fi
	sleep 1
	i=$((i+1))
done
[ -n "$ok" ] || pc_fail "process $z was not marked defunct: '$(cat out)'"
pc_pass
