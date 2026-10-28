# - as catfile and as msgfile
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.gencat.004
# A catfile of - writes the catalog to standard output and a msgfile of -
# reads standard input.
. "$PC_CORE/pc.sh"
pc_begin XCU.gencat.004
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
cat >m4.msg <<'EOM'
$set 1
1 hello
EOM
pc_test
gencat - m4.msg </dev/null >c4a.cat 2>err || pc_fail "gencat - m4.msg: nonzero status"
[ -s c4a.cat ] || pc_fail "gencat - wrote nothing to standard output"
./getmsg ./c4a.cat 1 1 >out 2>err || pc_fail "cannot read the catalog written to standard output"
[ "$(cat out)" = '[hello]' ] || pc_fail "catalog from standard output reads back as: $(cat out)"
gencat c4b.cat - <m4.msg >/dev/null 2>err || pc_fail "gencat c4b.cat -: nonzero status"
./getmsg ./c4b.cat 1 1 >out 2>err || pc_fail "cannot read the catalog built from standard input"
[ "$(cat out)" = '[hello]' ] || pc_fail "catalog from standard input reads back as: $(cat out)"
pc_pass
