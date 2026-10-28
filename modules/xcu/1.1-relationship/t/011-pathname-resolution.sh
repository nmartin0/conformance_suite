# Utilities resolve pathnames as the file hierarchy defines: dot, dot-dot, repeated slashes and symbolic link components work, a trailing slash after a non-directory and an empty pathname are errors
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.1-relationship.011
. "$PC_CORE/pc.sh"
pc_begin XCU.1.1-relationship.011
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mkdir -p a/b || pc_unresolved "mkdir"
printf 'x\n' >a/b/f || pc_unresolved "write"
sl=0
ln -s a lnk 2>/dev/null && [ -h lnk ] && sl=1
pc_test
for p in ./a/b/f a//b//f a/b/../b/f a/./b/f a/b/../../a/b/f ./././a/b/f; do
	[ "$(cat $p 2>/dev/null)" = x ] || pc_fail "cat $p did not read the file"
done
if [ "$sl" = 1 ]; then
	for p in lnk/b/f lnk/./b/../b/f ./lnk//b/f; do
		[ "$(cat $p 2>/dev/null)" = x ] || pc_fail "cat $p did not read the file"
	done
fi
cat a/b/f/ >o 2>e && pc_fail "a trailing slash after a regular file was accepted"
cat "" >o 2>e && pc_fail "an empty pathname was accepted"
case $(cd a/b/.. && pwd -P) in
*/a) ;;
*) pc_fail "dot-dot did not lead to the parent directory" ;;
esac
pc_pass
