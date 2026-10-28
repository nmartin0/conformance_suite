# With -R, -P changes a symbolic link found in the hierarchy without following it, and -L follows it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chgrp.004
. "$PC_CORE/pc.sh"
pc_begin XCU.chgrp.004
ids() {
	set -- $(ls -ldn "$1") || return 1
	iu=$3
	ig=$4
}

mkdir out top || pc_fail "cannot create directories"
: >out/g || pc_fail "cannot create out/g"
ln -s ../out/g top/sl || pc_fail "cannot create symbolic link"
: >top/f || pc_fail "cannot create top/f"
ids out/g || pc_fail "cannot list out/g"
g0=$ig
pc_test
chgrp -R -P 29999 top >out2 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_unresolved "chgrp -R -P exited with status $rc (links may not have groups here)"
ids top/f || pc_fail "cannot list top/f"
[ "$ig" = 29999 ] || pc_fail "top/f has group $ig"
ids out/g || pc_fail "cannot list out/g"
[ "$ig" = "$g0" ] || pc_fail "-P followed the link and changed out/g to $ig"
chgrp -R -L 29998 top >out2 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "chgrp -R -L exited with status $rc"
ids out/g || pc_fail "cannot list out/g"
[ "$ig" = 29998 ] || pc_fail "-L did not follow the link: out/g has group $ig"
pc_pass
