# touch -r combined with -a changes only the access time, taking it from the reference file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.touch.010
# MUTANT    touch.ar_both
# MUTANT    touch.ar_mtime_from_ref
# MUTANT    touch.ar_atime_now
# MUTANT    touch.ar_mtime_now
. "$PC_CORE/pc.sh"
pc_begin XCU.touch.010
TZ=UTC0; export TZ
nl='
'
same() { [ -e "$1" ] && [ -e "$2" ] && [ ! "$1" -nt "$2" ] && [ ! "$2" -nt "$1" ]; }
# atime_between f lo hi: the access time of f lies strictly between those of lo and hi
atime_between() { [ "$(ls -1ut "$3" "$1" "$2" 2>/dev/null)" = "$3$nl$1$nl$2" ]; }
touch -t 200101021504.30 ref || pc_fail "cannot create ref"
touch -a -t 200205060708.40 ref || pc_fail "cannot set the access time of ref"
touch -t 200205060708.39 lo || pc_fail "cannot create lo"
touch -t 200205060708.41 hi || pc_fail "cannot create hi"
touch -t 200301010101 f || pc_fail "cannot create f"
touch -t 200301010101 f0 || pc_fail "cannot create f0"
pc_test
touch -a -r ref f >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "touch -a -r exited with status $rc"
atime_between f lo hi || pc_fail "the access time is not that of the reference"
same f f0 || pc_fail "the modification time changed"
pc_pass
