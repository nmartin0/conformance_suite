# External C names that differ at the 31st byte, or only in case, stay distinct.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.2-conformance.008
. "$PC_CORE/pc.sh"
pc_begin XBD.2-conformance.008
LC_ALL=POSIX
export LC_ALL
cat >a.c <<'EOC' || pc_unresolved "cannot create a.c"
int abcdefghijklmnopqrstuvwxyz0123x = 11;
int abcdefghijklmnopqrstuvwxyz0123y = 22;
int CaseSym = 33;
int casesym = 44;
EOC
cat >b.c <<'EOC' || pc_unresolved "cannot create b.c"
#include <stdio.h>
extern int abcdefghijklmnopqrstuvwxyz0123x;
extern int abcdefghijklmnopqrstuvwxyz0123y;
extern int CaseSym;
extern int casesym;
int main(void)
{
	printf("%d %d %d %d\n", abcdefghijklmnopqrstuvwxyz0123x,
	    abcdefghijklmnopqrstuvwxyz0123y, CaseSym, casesym);
	return 0;
}
EOC
pc_test
# Without c17 the names cannot be tried; its absence is XBD.2-conformance.002's finding.
command -v c17 >/dev/null 2>&1 </dev/null || pc_unresolved "the c17 utility was not found"
c17 -o prog a.c b.c >cc.out 2>&1 </dev/null || pc_fail "c17 could not build a program from two files that define and use four external names differing at byte 31 or in case only"
got=$(./prog 2>/dev/null </dev/null)
[ "$got" = "11 22 33 44" ] || pc_fail "the program printed '$got', expected 11 22 33 44"
pc_pass
