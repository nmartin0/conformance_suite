# touch -r copies both the modification and the access time of the reference file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# The reference has different modification and access times; the expected
# values are files made in the setup phase.
#
# ASSERTION XCU.touch.009
# MUTANT    touch.r_atime_now
# MUTANT    touch.r_mtime_now
# MUTANT    touch.r_ignored
# MUTANT    touch.time_mtime_only
# MUTANT    touch.time_atime_only
# MUTANT    touch.new_file_current_time
# MUTANT    touch.only_first_operand
. "$PC_CORE/pc.sh"
pc_begin XCU.touch.009
TZ=UTC0; export TZ
nl='
'
same() { [ -e "$1" ] && [ -e "$2" ] && [ ! "$1" -nt "$2" ] && [ ! "$2" -nt "$1" ]; }
# atime_between f lo hi: the access time of f lies strictly between those of lo and hi
atime_between() { [ "$(ls -1ut "$3" "$1" "$2" 2>/dev/null)" = "$3$nl$1$nl$2" ]; }
touch -t 200101021504.30 ref || pc_fail "cannot create ref"
touch -a -t 200205060708.40 ref || pc_fail "cannot set the access time of ref"
touch -t 200101021504.30 rm || pc_fail "cannot create rm"
touch -t 200205060708.39 lo || pc_fail "cannot create lo"
touch -t 200205060708.41 hi || pc_fail "cannot create hi"
: >f || pc_fail "cannot create f"
: >g || pc_fail "cannot create g"
pc_test
touch -r ref f g newf >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "touch -r exited with status $rc"
for x in f g newf
do
	[ -f $x ] || pc_fail "$x does not exist"
	same $x rm || pc_fail "$x: the modification time is not that of the reference"
	atime_between $x lo hi || pc_fail "$x: the access time is not that of the reference"
done
same ref rm || pc_fail "the modification time of the reference changed"
atime_between ref lo hi || pc_fail "the access time of the reference changed"
pc_pass
