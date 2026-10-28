# Removing a file takes its name away and decrements its link count; the contents of a removed file stay readable through a descriptor opened earlier; rmdir removes an empty directory; and removing a name from a directory updates that directory's modification time
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.1-relationship.010
. "$PC_CORE/pc.sh"
pc_begin XCU.1.1-relationship.010
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'c\n' >f || pc_unresolved "write"
ln f g || pc_unresolved "cannot make a hard link"
set -- $(ls -ld g)
[ "$2" = 2 ] || pc_unresolved "link count is $2, not 2"
printf 'keep\n' >h || pc_unresolved "write"
mkdir dd || pc_unresolved "mkdir"
: >dd/x || pc_unresolved "write"
: >ref || pc_unresolved "write"
pc_test
rm f || pc_fail "rm failed"
[ ! -e f ] || pc_fail "the name still exists after rm"
set -- $(ls -ld g)
[ "$2" = 1 ] || pc_fail "the link count after rm is $2, not 1"
[ "$(cat g)" = c ] || pc_fail "the other link lost its contents"
exec 3<h
rm h || pc_fail "rm failed"
[ "$(cat <&3)" = keep ] || pc_fail "the contents of a removed open file were lost"
exec 3<&-
mkdir e || pc_unresolved "mkdir"
rmdir e || pc_fail "rmdir failed"
[ ! -e e ] || pc_fail "the directory still exists after rmdir"
sleep 2
rm dd/x || pc_fail "rm failed"
[ "$(find dd -newer ref -print)" = dd ] || pc_fail "the directory's modification time was not updated"
pc_pass
