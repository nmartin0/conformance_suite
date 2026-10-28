# Utilities whose STDIN section reads "Not used" do not read standard input when used as described: data waiting in a pipe is still there afterwards
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.4-utility-defaults.007
. "$PC_CORE/pc.sh"
pc_begin XCU.1.4-utility-defaults.007
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
: >t0 || pc_unresolved "cannot create a file"
pc_test
chk() {
	got=$(printf 'KEEP\n' | { "$@" >/dev/null 2>&1; cat; })
	[ "$got" = KEEP ] || pc_fail "$* read the standard input (left: '$got')"
}
chk ls .
chk date
chk echo hi
chk printf x
chk sleep 0
chk mkdir d1
chk rmdir d1
chk touch t1
chk ln t1 t2
chk chmod 600 t1
chk basename /a/b
chk dirname /a/b
chk id
chk uname
chk pwd
chk expr 1 + 1
chk df .
chk realpath .
chk mkfifo ff
chk env true
chk nice true
chk time true
chk timeout 5 true
chk test -d .
chk true
chk false
chk kill -0 "$$"
chk getconf PATH
chk who
pc_pass
