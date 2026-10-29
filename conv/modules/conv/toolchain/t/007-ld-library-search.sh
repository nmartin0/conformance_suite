# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION CONV.toolchain.007
#
# ld -lNAME looks in each -L directory in order, and in a directory takes
# libNAME.so before libNAME.a; the first directory that has either wins.
. "$PC_CORE/pc.sh"
. "${PC_CORE%/core}/conv/lib/tc.sh"
tc_begin CONV.toolchain.007
tc_need_ld
printf 'int tcxf(void);\nint tcstart(void) { return tcxf(); }\n' >m.c
printf 'int tcxf(void) { return 1; }\n' >x.c
tc_cobj m.c
tc_cobj x.c
mkdir d1 d2
# d1: both forms (shared object named tcA.so.1); d2 holds a shared object named tcB.so.1
tc_must "making d1/libx.so" "$LD" -shared -soname tcA.so.1 -o d1/libx.so x.o
tc_must "making d1/libx.a" "$AR" -rc d1/libx.a x.o
tc_must "making d2/libx.so" "$LD" -shared -soname tcB.so.1 -o d2/libx.so x.o
mkdir d3
tc_must "making d3/libx.a" "$AR" -rc d3/libx.a x.o
pc_test
needed() { # OUTFILE
	tc_dump "$1"
	tc_dyn "$1" NEEDED | tr '\n' ' '
}
tc_do "$LD" -e tcstart -o o1 m.o -L d1 -lx || pc_fail "link with a directory holding libx.so and libx.a failed: $(tc_out)"
n=$(needed o1)
case $n in *tcA.so.1*) ;; *) pc_fail "with libx.so and libx.a in one directory, the shared object was not chosen (needed: ${n:-none})" ;; esac
tc_do "$LD" -e tcstart -o o2 m.o -L d3 -L d2 -lx || pc_fail "link with the archive directory first failed: $(tc_out)"
n=$(needed o2)
case $n in *tcB.so.1*) pc_fail "the first directory (archive only) did not win: the shared object of the second directory was taken" ;; esac
l=$(tc_sym o2 tcxf)
[ "$(tc_key "$l" shndx)" != UND ] || pc_fail "tcxf is undefined: the archive of the first directory was not used"
tc_do "$LD" -e tcstart -o o3 m.o -L d2 -L d3 -lx || pc_fail "link with the shared directory first failed: $(tc_out)"
n=$(needed o3)
case $n in *tcB.so.1*) ;; *) pc_fail "the first directory (shared object) did not win (needed: ${n:-none})" ;; esac
pc_pass
