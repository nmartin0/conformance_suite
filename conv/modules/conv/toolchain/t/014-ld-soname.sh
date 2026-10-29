# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION CONV.toolchain.014
#
# ld -soname NAME records NAME as the shared object's name in its dynamic
# section, and a program linked against it records that name, not the file
# name, as its dependency.
. "$PC_CORE/pc.sh"
. "${PC_CORE%/core}/conv/lib/tc.sh"
tc_begin CONV.toolchain.014
tc_need_ld
printf 'int tcq(void) { return 1; }\n' >q.c
printf 'int tcq(void);\nint tcstart(void) { return tcq(); }\n' >m.c
tc_cobj q.c
tc_cobj m.c
pc_test
tc_do "$LD" -shared -soname tcname.so.7 -o libtcq.so q.o || pc_fail "ld -shared -soname failed: $(tc_out)"
tc_dump libtcq.so
s=$(tc_dyn libtcq.so SONAME)
[ "$s" = tcname.so.7 ] || pc_fail "the shared object records name '${s:-none}', not tcname.so.7"
tc_do "$LD" -e tcstart -o prog m.o -L . -ltcq || pc_fail "linking against the shared object failed: $(tc_out)"
tc_dump prog
n=$(tc_dyn prog NEEDED | tr '\n' ' ')
[ "$n" = "tcname.so.7 " ] || pc_fail "the program records dependencies '${n:-none}', not tcname.so.7"
pc_pass
