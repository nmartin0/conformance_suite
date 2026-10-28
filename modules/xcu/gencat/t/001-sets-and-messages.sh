# building a catalog from $set, comments and messages
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.gencat.001
# $set selects the set for the following messages, $ lines followed by a
# blank are comments, empty lines are ignored, and every message is
# retrievable by its set and message number.
. "$PC_CORE/pc.sh"
pc_begin XCU.gencat.001
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
cat >m1.msg <<'EOM'
$ a comment line
$set 1 first set

1 hello
2 world
$set 3 third set
1 three-one
EOM
pc_test
gencat c1.cat m1.msg </dev/null >out 2>err || pc_fail "gencat: nonzero status"
[ -s c1.cat ] || pc_fail "gencat wrote no catalog"
./getmsg ./c1.cat 1 1 1 2 3 1 1 3 2 1 >out 2>err || pc_fail "cannot read the catalog back"
printf '[hello]\n[world]\n[three-one]\n[<none>]\n[<none>]\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "messages read back as: $(tr '\n' ' ' <out)"
pc_pass
