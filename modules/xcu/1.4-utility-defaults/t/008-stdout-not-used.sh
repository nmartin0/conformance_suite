# Utilities whose STDOUT section reads "Not used" write nothing to standard output when used as described
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.4-utility-defaults.008
. "$PC_CORE/pc.sh"
pc_begin XCU.1.4-utility-defaults.008
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'x\n' >f1 || pc_unresolved "cannot create a file"
gid=$(id -g) || pc_unresolved "id -g"
uid=$(id -u) || pc_unresolved "id -u"
pc_test
chk() {
	"$@" >out 2>/dev/null </dev/null
	[ ! -s out ] || pc_fail "$* wrote to standard output: $(cat out)"
}
chk chmod 600 f1
chk chgrp "$gid" f1
chk chown "$uid" f1
chk cp f1 f2
chk mkdir d1
chk mv f2 f3
chk rmdir d1
chk ln f1 f4
chk touch f5
chk sleep 0
chk true
chk false
chk mkfifo ff
chk test -f f1
chk '[' -f f1 ']'
chk nice true
chk time true
chk timeout 5 true
chk pathchk f1
chk getopts a opt -a
chk unalias -a
chk wait
pc_pass
