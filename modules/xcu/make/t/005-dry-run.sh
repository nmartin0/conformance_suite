# make -n
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.005
# With -n the commands are written but not run (including @ lines, which are
# written), except that a + prefixed line is run.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
t:
	touch ran-plain
	@touch ran-at
	+touch ran-plus
EOM
pc_test
make -n t >o 2>err </dev/null || pc_fail "make -n: nonzero status"
sed 's/^[[:blank:]]*//' o >o.s
grep -x 'touch ran-plain' o.s >/dev/null || pc_fail "-n did not write the plain command"
grep -x 'touch ran-at' o.s >/dev/null || pc_fail "-n did not write the @ command"
[ -e ran-plain ] && pc_fail "-n ran a plain command"
[ -e ran-at ] && pc_fail "-n ran an @ command"
[ -e ran-plus ] || pc_fail "-n did not run the + command"
pc_pass
