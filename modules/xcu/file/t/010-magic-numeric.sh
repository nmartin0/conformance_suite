# A numeric test compares the value read from the file at the offset, here one byte.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.file.010
. "$PC_CORE/pc.sh"
pc_begin XCU.file.010
printf '0\tbyte\t65\tbyteA\n' >magic || pc_fail "cannot create magic"
printf 'ABC' >f1 || pc_fail "cannot create f1"
printf 'XBC' >f2 || pc_fail "cannot create f2"
pc_test
file -m magic f1 >out 2>err </dev/null
l=$(cat out)
case $l in
"f1: "*byteA*) ;;
*) pc_fail "matching file: $l" ;;
esac
file -m magic f2 >out 2>err </dev/null
l=$(cat out)
case $l in
*byteA*) pc_fail "non-matching file: $l" ;;
esac
pc_pass
