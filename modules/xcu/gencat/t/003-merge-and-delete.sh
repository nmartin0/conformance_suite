# merging into an existing catalog, replacing, deleting
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.gencat.003
# A second gencat run keeps the old messages, replaces colliding numbers, a
# bare message number deletes that message, and $delset removes a whole set.
. "$PC_CORE/pc.sh"
pc_begin XCU.gencat.003
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
cat >a.msg <<'EOM'
$set 1
1 one
2 two
3 three
$set 2
1 other
EOM
cat >b.msg <<'EOM'
$set 1
2 TWO
3
4 four
$delset 2
EOM
pc_test
gencat c3.cat a.msg </dev/null >out 2>err || pc_fail "first gencat: nonzero status"
gencat c3.cat b.msg </dev/null >out 2>err || pc_fail "second gencat: nonzero status"
./getmsg ./c3.cat 1 1 1 2 1 3 1 4 2 1 >out 2>err || pc_fail "cannot read the catalog back"
printf '[one]\n[TWO]\n[<none>]\n[four]\n[<none>]\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "merged catalog reads back as: $(tr '\n' ' ' <out)"
pc_pass
