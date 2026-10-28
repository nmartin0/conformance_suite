# pax: intermediate directories are made with the umask applied to 0777
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.020
# When an archive holds only the member a/b/c, extracting it creates the
# missing directories a and a/b with the mode the umask leaves of 0777
# (umask 027 gives drwxr-x---, umask 002 gives drwxrwxr-x).
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.020
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'umask 022; cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mkdir -p a/b || pc_unresolved "setup: mkdir failed"
printf 'x\n' >a/b/c || pc_unresolved "setup: cannot write a/b/c"
pax -w a/b/c >arch 2>/dev/null </dev/null || pc_unresolved "setup: pax -w failed"
n=$(pax <arch 2>/dev/null | wc -l | tr -d ' ')
[ "$n" -eq 1 ] || pc_unresolved "setup: the archive does not hold exactly the one member a/b/c"
pc_test
for pair in 027:drwxr-x--- 002:drwxrwxr-x; do
	um=${pair%%:*}; want=${pair#*:}
	rm -rf a
	umask "$um"
	pax -r <arch >o 2>err
	rc=$?
	umask 022
	[ "$rc" -eq 0 ] || pc_fail "umask $um: pax -r exit status $rc"
	[ "$(cat a/b/c 2>/dev/null)" = x ] || pc_fail "umask $um: a/b/c was not extracted"
	for d in a a/b; do
		got=$(ls -ld "$d" | cut -c1-10)
		[ "$got" = "$want" ] || pc_fail "umask $um: intermediate directory $d has mode $got, expected $want"
	done
done
pc_pass
