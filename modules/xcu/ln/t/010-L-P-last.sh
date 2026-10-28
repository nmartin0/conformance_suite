# When -L and -P are both given, the last one decides.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ln.010
. "$PC_CORE/pc.sh"
pc_begin XCU.ln.010
printf 'd\n' >f || pc_fail "cannot create f"
ln -s f sl || pc_fail "cannot create symbolic link"
pc_test
ln -P -L sl h1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -P -L exited with status $rc"
[ ! -h h1 ] || pc_fail "-P -L: h1 is a symbolic link"
[ h1 -ef f ] || pc_fail "-P -L: h1 is not a link to f"
ln -L -P sl h2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_unresolved "ln -L -P exited with status $rc (the file system may not allow links to symbolic links)"
[ -h h2 ] || pc_fail "-L -P: h2 is not a symbolic link"
pc_pass
