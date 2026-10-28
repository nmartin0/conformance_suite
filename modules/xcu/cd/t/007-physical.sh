# With -P links are resolved.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cd.007
. "$PC_CORE/pc.sh"
pc_begin XCU.cd.007
unset CDPATH
mkdir a b b/real || pc_fail "cannot make directories"
: >a/ma && : >b/mb && : >b/real/mr || pc_fail "cannot make marker files"
ln -s ../b/real a/link || pc_fail "cannot make a symbolic link"
base=$PWD
pc_test
cd -P a/link || pc_fail "cd -P a/link failed"
[ -f mr ] || pc_fail "not in the link target after cd -P a/link"
case $PWD in
*/b/real) ;;
*) pc_fail "PWD is '$PWD', wanted a physical path ending in /b/real" ;;
esac
cd -P .. || pc_fail "cd -P .. failed"
[ -f mb ] || pc_fail "cd -P .. did not go to the parent of the link target"
cd "$base" || pc_fail "cd to the base failed"
cd -P a/link/.. || pc_fail "cd -P a/link/.. failed"
[ -f mb ] || pc_fail "cd -P a/link/.. did not go to the parent of the link target"
pc_pass
