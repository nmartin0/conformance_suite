# ls -i -s with -l and -1: the numbers come first, in that order
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.044
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.044
LC_ALL=POSIX; export LC_ALL
mkdir d || pc_unresolved "cannot create d"
printf 'abcde' >d/f || pc_unresolved "cannot create d/f"
chmod 640 d/f || pc_unresolved "chmod failed"
ino=$(ls -i d/f | awk '{ print $1 }')
blk=$(ls -s d/f | awk '{ print $1 }')
case $ino in ''|*[!0-9]*) pc_unresolved "ls -i gave '$ino'" ;; esac
case $blk in ''|*[!0-9]*) pc_unresolved "ls -s gave '$blk'" ;; esac
pc_test
ls -i -s -l d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -i -s -l exited with status $rc"
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
ls -i -s -1 d >out 2>err </dev/null || pc_fail "ls -i -s -1 failed"
set -- $(cat out)
[ "$#" -eq 3 ] && [ "$1" = "$ino" ] && [ "$2" = "$blk" ] && [ "$3" = f ] || pc_fail "ls -i -s -1 wrote '$(cat out)', expected '$ino $blk f'"
pc_pass
