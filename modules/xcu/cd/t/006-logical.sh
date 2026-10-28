# By default a symbolic link stays in PWD and dot-dot is logical.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cd.006
. "$PC_CORE/pc.sh"
pc_begin XCU.cd.006
unset CDPATH
mkdir a b b/real || pc_fail "cannot make directories"
: >a/ma && : >b/mb && : >b/real/mr || pc_fail "cannot make marker files"
ln -s ../b/real a/link || pc_fail "cannot make a symbolic link"
base=$PWD
pc_test
cd a/link || pc_fail "cd a/link failed"
[ -f mr ] || pc_fail "not in the link target after cd a/link"
[ "$PWD" = "$base/a/link" ] || pc_fail "PWD is '$PWD', wanted $base/a/link"
cd .. || pc_fail "cd .. failed"
[ -f ma ] || pc_fail "cd .. from the link did not return to the directory holding the link"
[ "$PWD" = "$base/a" ] || pc_fail "PWD is '$PWD' after cd .., wanted $base/a"
cd "$base" || pc_fail "cd to the base failed"
cd -L a/link || pc_fail "cd -L a/link failed"
[ "$PWD" = "$base/a/link" ] || pc_fail "with -L PWD is '$PWD', wanted $base/a/link"
cd -L .. || pc_fail "cd -L .. failed"
[ -f ma ] || pc_fail "cd -L .. did not return to the directory holding the link"
pc_pass
