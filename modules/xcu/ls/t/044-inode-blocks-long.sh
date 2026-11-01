# ls -i -s with -l and -1: the numbers come first, in that order
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.044
# MUTANT    ls.ino_blk_swapped
# MUTANT    ls.ino_after_mode
# MUTANT    ls.ino_dropped_long
# MUTANT    ls.blk_dropped_long
# MUTANT    ls.no_total_s1
# MUTANT    ls.blk_after_name_1
# VARIANT   ls.s_units
# VARIANT   ls.dot_A
# VARIANT   ls.dot_a
# VARIANT   ls.acl_flag
# VARIANT   ls.names_numeric
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.044
LC_ALL=POSIX; export LC_ALL
mkdir d || pc_unresolved "cannot create d"
printf 'abcde' >d/f || pc_unresolved "cannot create d/f"
chmod 640 d/f || pc_unresolved "chmod failed"
pc_test
# the reference values come from the same ls, in the test phase
ino=$(ls -i d/f | awk '{ print $1 }')
blk=$(ls -s d/f | awk '{ print $1 }')
case $ino in ''|*[!0-9]*) pc_fail "ls -i gave '$ino'" ;; esac
case $blk in ''|*[!0-9]*) pc_fail "ls -s gave '$blk'" ;; esac
ls -i -s -l d >out0 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -i -s -l exited with status $rc"
# a listing may also hold dot and dot-dot (DESCRIPTION)
awk '$NF != "." && $NF != ".."' out0 >out
[ "$(sed -n '$=' out)" = 2 ] || pc_fail "expected the total line and one line: $(cat out)"
case $(sed -n 1p out) in
"total "[0-9]*) ;;
*) pc_fail "the first line is not a total line: $(sed -n 1p out)" ;;
esac
set -- $(sed -n 2p out)
[ "$#" -eq 11 ] || pc_fail "expected 11 fields, found $#: $(sed -n 2p out)"
[ "$1" = "$ino" ] || pc_fail "the first field '$1' is not the serial number $ino"
[ "$2" = "$blk" ] || pc_fail "the second field '$2' is not the block count $blk"
m10=${3%"${3#??????????}"}
[ "$m10" = '-rw-r-----' ] || pc_fail "the third field '$3' is not the mode string"
[ "${11}" = f ] || pc_fail "the last field is '${11}', expected f"
ls -i -s -1 d >out0 2>err </dev/null || pc_fail "ls -i -s -1 failed"
# a listing may also hold dot and dot-dot (DESCRIPTION)
awk '$NF != "." && $NF != ".."' out0 >out
# -s also puts the status line "total N" before the list of a directory
case $(sed -n 1p out) in
"total "[0-9]*) ;;
*) pc_fail "the first line of ls -i -s -1 is not a total line: $(sed -n 1p out)" ;;
esac
[ "$(sed -n '$=' out)" = 2 ] || pc_fail "ls -i -s -1 wrote '$(cat out)', expected the total line and one line"
set -- $(sed -n 2p out)
[ "$#" -eq 3 ] && [ "$1" = "$ino" ] && [ "$2" = "$blk" ] && [ "$3" = f ] || pc_fail "ls -i -s -1 wrote '$(sed -n 2p out)', expected '$ino $blk f'"
pc_pass
