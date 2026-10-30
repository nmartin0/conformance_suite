# Utilities that run other utilities (env, xargs, find -exec, and nohup, nice and time where present) can run echo, printf, true, test, kill, basename, dirname and expr, and false gives status 1 through env
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.6-built-in-utilities.002
. "$PC_CORE/pc.sh"
pc_begin XCU.1.6-built-in-utilities.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
pc_test
t1() {
	lab=$1; exp=$2; shift 2
	"$@" >out 2>err </dev/null || pc_fail "$lab: failed: $(dd bs=200 count=1 <err 2>/dev/null)"
	[ "$(cat out)" = "$exp" ] || pc_fail "$lab: printed '$(cat out)', wanted '$exp'"
}
all() {
	v=$1; exp=$2; shift 2
	t1 "env $*" "$exp" env "$@" "$v"
	printf '%s\n' "$v" | xargs "$@" >out 2>err || pc_fail "xargs $*: failed: $(dd bs=200 count=1 <err 2>/dev/null)"
	[ "$(cat out)" = "$exp" ] || pc_fail "xargs $*: printed '$(cat out)', wanted '$exp'"
	t1 "find $*" "$exp" find . -prune -exec "$@" "$v" ';'
	for w in nohup nice time; do
		command -v "$w" >/dev/null 2>&1 || continue
		t1 "$w $*" "$exp" "$w" "$@" "$v"
	done
}
all x 'pre x' echo pre
all q 'q|' printf '%s|\n'
all x '' true
all 1 '' test 1 -eq
all "$$" '' kill -s 0
all a/b.c b.c basename
all a/b.c a dirname
all 4 7 expr 3 +
env false </dev/null >/dev/null 2>&1
[ $? = 1 ] || pc_fail "env false did not give status 1"
pc_pass
