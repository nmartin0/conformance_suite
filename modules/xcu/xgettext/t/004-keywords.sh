# -K keyword specifications
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.xgettext.004
# -K id extracts the first argument of id, id:2 the second, id:1,2 a
# singular and plural pair, and an empty -K disables the default keywords.
. "$PC_CORE/pc.sh"
pc_begin XCU.xgettext.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
ids() { grep '^msgid "[^"]' "$1" | sed 's/^msgid "//; s/"$//' | tr '\n' ' '; }
cat >t.c <<'EOC'
void f(void)
{
	gettext("gt");
	tr("first");
	dt("dom", "second");
	pl("sing", "plur", 1);
}
EOC
pc_test
xgettext -K tr t.c </dev/null >o 2>e || pc_fail "-K tr: nonzero status"
[ "$(ids messages.po)" = 'gt first ' ] || pc_fail "-K tr extracted: $(ids messages.po)"
rm -f messages.po
xgettext -K dt:2 t.c </dev/null >o 2>e || pc_fail "-K dt:2: nonzero status"
[ "$(ids messages.po)" = 'gt second ' ] || pc_fail "-K dt:2 extracted: $(ids messages.po)"
rm -f messages.po
xgettext -K pl:1,2 t.c </dev/null >o 2>e || pc_fail "-K pl:1,2: nonzero status"
[ "$(ids messages.po)" = 'gt sing ' ] || pc_fail "-K pl:1,2 extracted: $(ids messages.po)"
grep -x 'msgid_plural "plur"' messages.po >/dev/null || pc_fail "-K pl:1,2 wrote no msgid_plural"
rm -f messages.po
xgettext -K '' -K tr t.c </dev/null >o 2>e || pc_fail "-K '' -K tr: nonzero status"
[ "$(ids messages.po)" = 'first ' ] || pc_fail "an empty -K did not disable the defaults: $(ids messages.po)"
pc_pass
