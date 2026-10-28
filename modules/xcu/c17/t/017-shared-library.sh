# -G, -B dynamic and -R
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.c17.017
# -G creates a shared library (and with -c suitable objects), a program is
# linked with -B dynamic, -L, -l and -R against it, and a shared library is
# preferred to an archive of the same name when linking dynamically.
. "$PC_CORE/pc.sh"
pc_begin XCU.c17.017
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v ar >/dev/null 2>&1 || pc_unresolved "setup: ar is not available"
mkdir lib || pc_unresolved "setup: mkdir failed"
printf 'int pcf(void) { return 2; }\n' >f.c
printf 'int pcf(void) { return 1; }\n' >g.c
printf 'int pcf(void);\nint main(void) { return pcf(); }\n' >main.c
pc_test
c17 -G -c f.c </dev/null >out 2>err || pc_fail "c17 -G -c failed"
[ -f f.o ] || pc_fail "c17 -G -c made no f.o"
c17 -G -o lib/libpcx.so.1 f.o </dev/null >out 2>err || pc_fail "c17 -G -o libpcx.so.1 failed"
[ -s lib/libpcx.so.1 ] || pc_fail "no shared library was written"
cp lib/libpcx.so.1 lib/libpcx.so || pc_unresolved "setup: cp failed"
c17 -c g.c </dev/null >/dev/null 2>&1 || pc_unresolved "setup: c17 -c failed"
ar -rc lib/libpcx.a g.o </dev/null >/dev/null 2>&1 || pc_unresolved "setup: ar failed"
c17 -B dynamic -o p main.c -L lib -l pcx -R "$PWD/lib" </dev/null >out 2>err || pc_fail "linking with -B dynamic -L -l -R failed"
./p </dev/null >out 2>err
rc=$?
case $rc in
2) ;;
1) pc_fail "-B dynamic linked the archive although a shared library was present" ;;
*) pc_unresolved "the dynamically linked program could not run (status $rc); -R may not be supported by the object format" ;;
esac
pc_pass
