# c17 -D and -U
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.c17.003
# "-D NAME" defines NAME as 1 and "-D NAME=7" as 7; a name given to both -U
# and -D is undefined whichever comes first.
. "$PC_CORE/pc.sh"
pc_begin XCU.c17.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >d.c <<'EOC'
#include <stdio.h>
int main(void)
{
#ifdef PCF
	printf("F=%d ", PCF);
#endif
#ifdef PCV
	printf("V=%d ", PCV);
#endif
#ifdef PCU
	printf("U ");
#endif
	puts("end");
	return 0;
}
EOC
pc_test
c17 -D PCF -D PCV=7 -o p1 d.c >out 2>err </dev/null || pc_fail "-D: nonzero status"
[ "$(./p1 </dev/null)" = 'F=1 V=7 end' ] || pc_fail "-D definitions give '$(./p1 </dev/null)'"
c17 -D PCU=1 -U PCU -o p2 d.c >out 2>err </dev/null || pc_fail "-D then -U: nonzero status"
[ "$(./p2 </dev/null)" = end ] || pc_fail "-D PCU -U PCU left the name defined"
c17 -U PCU -D PCU=1 -o p3 d.c >out 2>err </dev/null || pc_fail "-U then -D: nonzero status"
[ "$(./p3 </dev/null)" = end ] || pc_fail "-U PCU -D PCU left the name defined"
pc_pass
