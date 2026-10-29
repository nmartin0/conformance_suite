# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION CONV.toolchain.021
#
# The initialisation functions listed in a shared library's init array run
# before the program's main function starts, and those of a library that
# another library depends on run before the dependent library's.
. "$PC_CORE/pc.sh"
. "${PC_CORE%/core}/conv/lib/tc.sh"
tc_begin CONV.toolchain.021
tc_need CC AS
tc_need_probe pic "-fPIC -shared"
tc_need_probe rpath "-Wl,-rpath"
tc_probe ptr
case $tc_pr in 4) w=.long a=2 ;; 8) w=.quad a=3 ;; *) pc_notinuse "pointer size $tc_pr is not handled" ;; esac
cat >b.c <<'EOT'
char tclog[16];
int tcn;
void tcnote(char c) { if (tcn < 15) tclog[tcn++] = c; }
void tcinitb(void) { tcnote('B'); }
EOT
cat >a.c <<'EOT'
extern char tclog[];
void tcnote(char c);
void tcinita(void) { tcnote('A'); }
const char *tcgetlog(void) { return tclog; }
EOT
for x in a b; do
	cat >${x}init.s <<EOT
	.section .init_array,"aw"
	.balign $((1 << a))
	$w tcinit$x
EOT
done
cat >main.c <<'EOT'
#include <stdio.h>
const char *tcgetlog(void);
int main(void) { printf("%s\n", tcgetlog()); return 0; }
EOT
tc_cobj b.c -fPIC
tc_cobj a.c -fPIC
tc_aobj binit.s
tc_aobj ainit.s
rp="-Wl,-rpath,$PWD"
tc_must "making the dependency" "$CC" -shared -o libtcB.so b.o binit.o
tc_must "making the dependent library" "$CC" -shared -o libtcA.so a.o ainit.o -L . -ltcB "$rp"
# the dependent library first on the program's link line, and last
tc_must "making the program" "$CC" -o prog main.c -L . -ltcA -ltcB "$rp"
tc_must "making the second program" "$CC" -o prog2 main.c -L . -ltcB -ltcA "$rp"
for f in libtcA.so libtcB.so; do
	tc_dump $f
	[ -n "$(awk '$1 == "DYN" && $2 == "INIT_ARRAY"' $f.dump)" ] || pc_unresolved "setup: $f has no init array"
done
pc_test
out=$(./prog 2>&1) || pc_fail "the program failed: $out"
[ "$out" = BA ] || pc_fail "at the start of main the initialisation log reads '$out': expected 'BA' (dependency first, both before main)"
out=$(./prog2 2>&1) || pc_fail "the second program failed: $out"
[ "$out" = BA ] || pc_fail "with the dependency listed first the log reads '$out': expected 'BA'"
pc_pass
