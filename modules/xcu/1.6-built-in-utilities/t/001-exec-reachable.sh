# Each standard utility that is neither a special built-in nor an intrinsic utility (except kill, which is one) exists as an executable file found by a search of PATH, and the file runs when named by its path: echo, printf, true, false, test, [, kill, basename, dirname and expr give their expected results
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.6-built-in-utilities.001
. "$PC_CORE/pc.sh"
pc_begin XCU.1.6-built-in-utilities.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
findx() {
	oldifs=$IFS
	IFS=:
	for d in $PATH; do
		[ -n "$d" ] || d=.
		if [ -f "$d/$1" ] && [ -x "$d/$1" ]; then
			IFS=$oldifs
			printf '%s\n' "$d/$1"
			return 0
		fi
	done
	IFS=$oldifs
	return 1
}
pc_test
for n in awk basename cat chgrp chmod chown cksum cmp comm cp cut date dd df diff dirname du echo ed env expr false find getconf grep head id kill ln ls mkdir mkfifo mv od paste pathchk printf pwd rm rmdir sed sleep sort split tail tee test touch tr true tty uname uniq wc xargs '['; do
	findx "$n" >/dev/null || pc_fail "no executable file named $n in a PATH directory"
done
run() {
	# run name expected-stdout expected-status args...
	n=$1; exp=$2; st=$3; shift 3
	f=$(findx "$n") || pc_fail "no executable file named $n"
	"$f" "$@" >out 2>err </dev/null
	rc=$?
	[ "$rc" = "$st" ] || pc_fail "$f exit status $rc, wanted $st"
	[ "$(cat out)" = "$exp" ] || pc_fail "$f printed '$(cat out)', wanted '$exp'"
}
run echo hi 0 hi
run printf 'a|' 0 '%s|' a
run true '' 0
run false '' 1
run test '' 0 1 -eq 1
run test '' 1 1 -eq 2
run '[' '' 0 1 -eq 1 ']'
run kill '' 0 -s 0 "$$"
run basename b.c 0 a/b.c
run dirname a 0 a/b.c
run expr 7 0 3 + 4
pc_pass
