# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION CONV.toolchain.018
#
# A run path that begins with $ORIGIN is taken relative to the directory of
# the file that contains it: the program finds its library after the whole
# tree is moved or started from another directory, and does not find it when
# the program is copied to a place where that relative path leads nowhere.
. "$PC_CORE/pc.sh"
. "${PC_CORE%/core}/conv/lib/tc.sh"
tc_begin CONV.toolchain.018
tc_need CC
tc_need_probe pic "-fPIC -shared"
tc_need_probe rpath "-Wl,-rpath"
mkdir t1 t1/bin t1/lib
printf 'int tcq(void) { return 7; }\n' >q.c
printf '#include <stdio.h>\nint tcq(void);\nint main(void) { printf("%%d\\n", tcq()); return 0; }\n' >main.c
tc_must "making the library" "$CC" -fPIC -shared -o t1/lib/libtcq18.so q.c
tc_must "making the program" "$CC" -o t1/bin/prog main.c -L t1/lib -ltcq18 '-Wl,-rpath,$ORIGIN/../lib'
pc_test
out=$(./t1/bin/prog 2>&1) || pc_fail "the program did not start with a run path of \$ORIGIN/../lib: $out"
[ "$out" = 7 ] || pc_fail "the program printed '$out', not 7"
out=$(cd t1/lib && ../bin/prog 2>&1) || pc_fail "the program did not start from another directory: $out"
[ "$out" = 7 ] || pc_fail "started from another directory it printed '$out', not 7"
mv t1 t2 || pc_unresolved "setup: cannot rename the tree"
out=$(./t2/bin/prog 2>&1) || pc_fail "the program did not start after the tree was moved: $out"
[ "$out" = 7 ] || pc_fail "after the move it printed '$out', not 7"
mkdir other other/bin
cp t2/bin/prog other/bin/prog || pc_unresolved "setup: cannot copy the program"
if out=$(./other/bin/prog 2>&1); then
	pc_fail "the copy in a tree without the library started ('$out'): \$ORIGIN does not follow the program"
fi
pc_pass
