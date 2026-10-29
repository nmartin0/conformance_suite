# For a symbolic link to a file, ls -l describes the link itself; with -L or -H it describes the file but still writes the name of the link.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.029
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.029
LC_ALL=POSIX; export LC_ALL
mkdir d || pc_fail "cannot create d"
: >d/f || pc_fail "cannot create d/f"
ln -s f d/lk || pc_fail "cannot create symbolic link"
pc_test
l=$(ls -l d/lk 2>err) || pc_fail "ls -l failed: $(cat err)"
case $l in
l*" d/lk -> f") ;;
*) pc_fail "ls -l: $l" ;;
esac
l=$(ls -lL d/lk 2>err) || pc_fail "ls -lL failed: $(cat err)"
case $l in
-*" d/lk") ;;
*) pc_fail "ls -lL: $l" ;;
esac
l=$(ls -lH d/lk 2>err) || pc_fail "ls -lH failed: $(cat err)"
case $l in
-*" d/lk") ;;
*) pc_fail "ls -lH: $l" ;;
esac
pc_pass
