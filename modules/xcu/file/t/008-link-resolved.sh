# By default file describes what a symbolic link refers to; a link to a missing file is described as a symbolic link.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.file.008
. "$PC_CORE/pc.sh"
pc_begin XCU.file.008
mkdir d || pc_fail "cannot create d"
: >e || pc_fail "cannot create e"
ln -s d ld || pc_fail "cannot create ld"
ln -s e le || pc_fail "cannot create le"
ln -s nowhere dl || pc_fail "cannot create dl"
pc_test
file ld le dl >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "file exited with status $rc"
l1=$(sed -n 1p out)
l2=$(sed -n 2p out)
l3=$(sed -n 3p out)
case $l1 in
"ld: "*directory*) ;;
*) pc_fail "link to a directory: $l1" ;;
esac
case $l2 in
"le: "*empty*) ;;
*) pc_fail "link to an empty file: $l2" ;;
esac
case $l3 in
"dl: "*"symbolic link to"*" nowhere") ;;
*) pc_fail "dangling link: $l3" ;;
esac
pc_pass
