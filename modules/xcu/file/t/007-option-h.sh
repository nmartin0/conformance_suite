# With -h a symbolic link is described as a symbolic link followed by its contents.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.file.007
. "$PC_CORE/pc.sh"
pc_begin XCU.file.007
printf 'x\n' >f || pc_fail "cannot create f"
ln -s f l || pc_fail "cannot create symbolic link"
pc_test
file -h l >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "file -h exited with status $rc"
l=$(cat out)
case $l in
"l: "*"symbolic link to"*" f") ;;
*) pc_fail "line is: $l" ;;
esac
pc_pass
