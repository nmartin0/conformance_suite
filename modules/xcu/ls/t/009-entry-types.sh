# The first character of the long format mode is d for a directory, p for a FIFO, l for a symbolic link and - for a regular file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.009
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.009
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
}

LC_ALL=POSIX; export LC_ALL
mkdir d d/sub || pc_fail "cannot create directories"
mkfifo d/p || pc_fail "cannot create FIFO"
: >d/f || pc_fail "cannot create d/f"
ln -s f d/l || pc_fail "cannot create symbolic link"
pc_test
for pair in sub:d p:p l:l f:-; do
	n=${pair%%:*}
	want=${pair##*:}
	modeof "d/$n" || pc_fail "cannot list d/$n"
	c=${mo%"${mo#?}"}
	[ "$c" = "$want" ] || pc_fail "d/$n has entry type '$c', expected '$want'"
done
pc_pass
