# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION CONV.toolchain.020
#
# A global function with default visibility that a shared library defines
# and also calls is preempted: when the program defines the same name, the
# library's own call goes to the program's definition. Without a definition
# in the program the library's definition is used.
. "$PC_CORE/pc.sh"
. "${PC_CORE%/core}/conv/lib/tc.sh"
tc_begin CONV.toolchain.020
tc_need CC
tc_need_probe pic "-fPIC -shared"
tc_need_probe rpath "-Wl,-rpath"
# the definition and the call are in different objects, so the compiler
# cannot bind the call before the linker and loader see it
printf 'int tcwho(void) { return 2; }\n' >w.c
printf 'int tcwho(void);\nint tclibcall(void) { return tcwho(); }\n' >call.c
cat >main1.c <<'EOT'
#include <stdio.h>
int tclibcall(void);
int tcwho(void) { return 1; }
int main(void) { printf("%d %d\n", tcwho(), tclibcall()); return 0; }
EOT
cat >main2.c <<'EOT'
#include <stdio.h>
int tclibcall(void);
int tcwho(void);
int main(void) { printf("%d %d\n", tcwho(), tclibcall()); return 0; }
EOT
tc_cobj w.c -fPIC
tc_cobj call.c -fPIC
tc_must "making the library" "$CC" -shared -o libtcq20.so w.o call.o
rp="-Wl,-rpath,$PWD"
tc_must "making the preempting program" "$CC" -o prog1 main1.c -L . -ltcq20 "$rp"
tc_must "making the control program" "$CC" -o prog2 main2.c -L . -ltcq20 "$rp"
pc_test
out=$(./prog2 2>&1) || pc_fail "the control program failed: $out"
[ "$out" = "2 2" ] || pc_fail "without a definition in the program the library gave '$out', not '2 2'"
out=$(./prog1 2>&1) || pc_fail "the program failed: $out"
[ "$out" = "1 1" ] || pc_fail "with a definition in the program the output is '$out': expected '1 1' (the library's call preempted)"
pc_pass
