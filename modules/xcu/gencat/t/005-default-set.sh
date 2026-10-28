# messages with no $set and several msgfiles
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.gencat.005
# Messages before any $set go to the default set NL_SETD, and several
# msgfile operands are merged into one catalog.
. "$PC_CORE/pc.sh"
pc_begin XCU.gencat.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >getmsg.c <<'EOC'
#include <nl_types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char **argv)
{
	nl_catd c;
	int i;

	if (argc < 2)
		return 2;
	c = catopen(argv[1], 0);
	if (c == (nl_catd)-1) {
		puts("OPENFAIL");
		return 2;
	}
	for (i = 2; i + 1 < argc; i += 2) {
		int set = strcmp(argv[i], "D") == 0 ? NL_SETD : atoi(argv[i]);

		printf("[%s]\n", catgets(c, set, atoi(argv[i + 1]), "<none>"));
	}
	catclose(c);
	return 0;
}
EOC
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
c17 -o getmsg getmsg.c </dev/null >cc.out 2>&1 || pc_unresolved "setup: cannot build the catalog reader"
cat >d.msg <<'EOM'
1 default
EOM
cat >e.msg <<'EOM'
$set 1
1 first
EOM
cat >f.msg <<'EOM'
$set 2
1 second
EOM
pc_test
gencat c5.cat d.msg </dev/null >out 2>err || pc_fail "gencat: nonzero status"
./getmsg ./c5.cat D 1 >out 2>err || pc_fail "cannot read the catalog back"
[ "$(cat out)" = '[default]' ] || pc_fail "the default set reads back as: $(cat out)"
gencat c5b.cat e.msg f.msg </dev/null >out 2>err || pc_fail "gencat with two msgfiles: nonzero status"
./getmsg ./c5b.cat 1 1 2 1 >out 2>err || pc_fail "cannot read the merged catalog"
[ "$(tr '\n' ' ' <out)" = '[first] [second] ' ] || pc_fail "two msgfiles read back as: $(tr '\n' ' ' <out)"
pc_pass
