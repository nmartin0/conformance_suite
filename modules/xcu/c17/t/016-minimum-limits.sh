# -D, -I and -L minimum counts
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.c17.016
# At least 256 -D names with more than 2048 bytes of definitions, ten -I
# options and ten -L options are accepted in one invocation.
. "$PC_CORE/pc.sh"
pc_begin XCU.c17.016
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v ar >/dev/null 2>&1 || pc_unresolved "setup: ar is not available"
defs=
k=1
while [ "$k" -le 256 ]; do
	defs="$defs -D N$k=1234"
	k=$((k+1))
done
printf 'int main(void) { return (N1 == 1234 && N128 == 1234 && N256 == 1234) ? 0 : 1; }\n' >a.c
k=1
incs=
libs=
while [ "$k" -le 10 ]; do
	mkdir i$k l$k || pc_unresolved "setup: mkdir failed"
	incs="$incs -I i$k"
	libs="$libs -L l$k"
	k=$((k+1))
done
printf '#define HV 9\n' >i10/h.h
printf '#include <h.h>\nint main(void) { return HV; }\n' >b.c
printf 'int pcf(void) { return 4; }\n' >f.c
printf 'int pcf(void);\nint main(void) { return pcf(); }\n' >c.c
c17 -c f.c </dev/null >/dev/null 2>&1 || pc_unresolved "setup: c17 -c failed"
ar -rc l10/libpcx.a f.o </dev/null >/dev/null 2>&1 || pc_unresolved "setup: ar failed"
pc_test
c17 $defs -o pa a.c </dev/null >out 2>err || pc_fail "256 -D options were not accepted"
./pa </dev/null
[ "$?" -eq 0 ] || pc_fail "the 256 definitions did not all take effect"
c17 $incs -o pb b.c </dev/null >out 2>err || pc_fail "ten -I options were not accepted"
./pb </dev/null
[ "$?" -eq 9 ] || pc_fail "the header in the tenth -I directory was not used"
c17 $libs -o pc c.c -l pcx </dev/null >out 2>err || pc_fail "ten -L options were not accepted"
./pc </dev/null
[ "$?" -eq 4 ] || pc_fail "the library in the tenth -L directory was not used"
pc_pass
