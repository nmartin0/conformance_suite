# chgrp -h changes the group of a symbolic link itself and not of the file it refers to; without -h the file is changed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chgrp.003
. "$PC_CORE/pc.sh"
pc_begin XCU.chgrp.003
ids() {
	set -- $(ls -ldn "$1") || return 1
	iu=$3
	ig=$4
}

: >f || pc_fail "cannot create f"
ln -s f l || pc_fail "cannot create symbolic link"
ids f || pc_fail "cannot list f"
g0=$ig
pc_test
chgrp -h 29999 l >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_unresolved "chgrp -h exited with status $rc (links may not have groups here)"
ids l || pc_fail "cannot list l"
[ "$ig" = 29999 ] || pc_fail "the link has group $ig, expected 29999"
ids f || pc_fail "cannot list f"
[ "$ig" = "$g0" ] || pc_fail "the file behind the link was changed to $ig"
chgrp 29998 l >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "chgrp through the link exited with status $rc"
ids f || pc_fail "cannot list f"
[ "$ig" = 29998 ] || pc_fail "without -h the file has group $ig, expected 29998"
pc_pass
