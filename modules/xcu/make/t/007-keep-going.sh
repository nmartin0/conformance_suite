# make -k and -S
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.007
# After a failure, make -k still builds independent targets and ends with
# nonzero status; without -k, or with -S after -k, it stops; with -S before
# -k it keeps going.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.007
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
all: bad good
bad:
	false
good:
	echo good-built
EOM
pc_test
make >o 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "failure without -k gave status 0"
grep -x 'good-built' o >/dev/null && pc_fail "without -k the next target was built after the failure"
make -k >o 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "-k: status 0 although a target failed"
grep -x 'good-built' o >/dev/null || pc_fail "-k did not build the independent target"
make -k -S >o 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "-k -S: status 0"
grep -x 'good-built' o >/dev/null && pc_fail "-k -S kept going (the last option is -S)"
make -S -k >o 2>err </dev/null
grep -x 'good-built' o >/dev/null || pc_fail "-S -k did not keep going (the last option is -k)"
pc_pass
