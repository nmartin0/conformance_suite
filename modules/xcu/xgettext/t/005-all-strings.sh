# -a
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.xgettext.005
# -a extracts every string literal and writes only one dot-po file.
. "$PC_CORE/pc.sh"
pc_begin XCU.xgettext.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
ids() { grep '^msgid "[^"]' "$1" | sed 's/^msgid "//; s/"$//' | tr '\n' ' '; }
cat >t.c <<'EOC'
#include <stdio.h>
void f(void)
{
	printf("plain one");
	puts("plain two");
	gettext("alpha");
}
EOC
pc_test
xgettext -a t.c </dev/null >o 2>e || pc_fail "xgettext -a: nonzero status"
[ -s messages.po ] || pc_fail "messages.po was not written"
got=$(ids messages.po)
case $got in *'plain one'*'plain two'*alpha*) ;; *) pc_fail "-a extracted: $got" ;; esac
n=$(ls *.po | wc -l)
[ "$n" -eq 1 ] || pc_fail "-a wrote $n dot-po files"
pc_pass
