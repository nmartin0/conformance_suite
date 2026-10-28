# With both -L and -P the last one decides.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cd.008
. "$PC_CORE/pc.sh"
pc_begin XCU.cd.008
unset CDPATH
mkdir a b b/real || pc_fail "cannot make directories"
ln -s ../b/real a/link || pc_fail "cannot make a symbolic link"
base=$PWD
pc_test
cd -P -L a/link || pc_fail "cd -P -L a/link failed"
[ "$PWD" = "$base/a/link" ] || pc_fail "after -P -L PWD is '$PWD', wanted $base/a/link"
cd "$base" || pc_fail "cd to the base failed"
cd -L -P a/link || pc_fail "cd -L -P a/link failed"
case $PWD in
*/b/real) ;;
*) pc_fail "after -L -P PWD is '$PWD', wanted a physical path ending in /b/real" ;;
esac
pc_pass
