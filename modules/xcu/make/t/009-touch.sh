# make -t
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.009
# "make -t" does not run an out-of-date target's commands but updates the
# target's modification time and writes a message naming it; a target with
# prerequisites but no commands is not touched.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.009
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
target1: src
	echo content > target1
grp: target1
EOM
printf 'orig\n' >target1
: >src
touch -t 200001010000 target1 || pc_unresolved "setup: touch failed"
touch -t 200101010000 src || pc_unresolved "setup: touch failed"
touch -t 200101010000 ref || pc_unresolved "setup: touch failed"
pc_test
make -t target1 >o 2>err </dev/null || pc_fail "make -t: nonzero status: $(sed 1q err)"
[ "$(cat target1)" = orig ] || pc_fail "-t ran the commands"
new=$(find target1 -newer ref -print)
[ -n "$new" ] || pc_fail "-t did not update the modification time"
grep -F target1 o >/dev/null || pc_fail "-t wrote no message naming the file"
make -t grp >o 2>err </dev/null
[ -e grp ] && pc_fail "-t touched a target that has no commands"
pc_pass
