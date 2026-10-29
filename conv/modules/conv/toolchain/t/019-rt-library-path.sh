# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION CONV.toolchain.019
#
# The environment variable LD_LIBRARY_PATH supplies extra directories (a
# colon-separated list searched in turn) in which the program's shared
# libraries are looked for; the library is found there although the program
# records no path to it.
. "$PC_CORE/pc.sh"
. "${PC_CORE%/core}/conv/lib/tc.sh"
tc_begin CONV.toolchain.019
tc_need CC
tc_need_probe pic "-fPIC -shared"
mkdir lib1 lib2
printf 'int tcq(void) { return 7; }\n' >q.c
printf '#include <stdio.h>\nint tcq(void);\nint main(void) { printf("%%d\\n", tcq()); return 0; }\n' >main.c
tc_must "making the library" "$CC" -fPIC -shared -o lib1/libtcq19.so q.c
tc_must "making the program" "$CC" -o prog main.c -L lib1 -ltcq19
mv lib1/libtcq19.so lib2/ || pc_unresolved "setup: cannot move the library"
if out=$(./prog 2>&1); then
	pc_unresolved "setup: the program started although its library is in no searched place ($out)"
fi
pc_test
d=$PWD
out=$(env LD_LIBRARY_PATH="$d/lib2" ./prog 2>&1) || pc_fail "LD_LIBRARY_PATH=lib2 did not let the program find its library: $out"
[ "$out" = 7 ] || pc_fail "with LD_LIBRARY_PATH the program printed '$out', not 7"
out=$(env LD_LIBRARY_PATH="$d/lib1:$d/nonesuch:$d/lib2" ./prog 2>&1) || pc_fail "a colon-separated LD_LIBRARY_PATH was not searched in turn: $out"
[ "$out" = 7 ] || pc_fail "with a list in LD_LIBRARY_PATH the program printed '$out', not 7"
pc_pass
