# pwd -L, and pwd without options, write the PWD pathname when it is valid, so a path through a symbolic link is kept.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pwd.003
#
# The shell sets PWD when it changes directory; the test only exports it
# for the direct exec and never assigns a value.
. "$PC_CORE/pc.sh"
pc_begin XCU.pwd.003
root=$(pwd -P) || pc_fail "cannot determine the directory"
mkdir real || pc_fail "cannot create real"
ln -s real lnk || pc_fail "cannot create symbolic link"
cd "$root/lnk" || pc_fail "cannot enter lnk"
pc_test
out=$(pwd -L)
[ "$out" = "$root/lnk" ] || pc_fail "pwd -L wrote '$out', expected '$root/lnk'"
out=$(pwd)
[ "$out" = "$root/lnk" ] || pc_fail "pwd wrote '$out', expected '$root/lnk'"
export PWD
out=$( (exec pwd -L) )
[ "$out" = "$root/lnk" ] || pc_fail "exec pwd -L wrote '$out', expected '$root/lnk'"
out=$( (exec pwd) )
[ "$out" = "$root/lnk" ] || pc_fail "exec pwd wrote '$out', expected '$root/lnk'"
pc_pass
