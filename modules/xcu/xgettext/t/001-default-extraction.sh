# gettext and ngettext strings to messages.po
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.xgettext.001
# String literals in gettext and ngettext calls are written to messages.po
# in source order as msgid directives with empty msgstr, plurals get
# msgid_plural and msgstr[0] directives, other strings are ignored, and a
# repeated msgid is written once uncommented.
. "$PC_CORE/pc.sh"
pc_begin XCU.xgettext.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
ids() { grep '^msgid "[^"]' "$1" | sed 's/^msgid "//; s/"$//' | tr '\n' ' '; }
cat >t.c <<'EOC'
#include <libintl.h>
void f(int n)
{
	gettext("alpha");
	printf("not extracted");
	ngettext("one file", "many files", n);
	gettext("beta");
	gettext("alpha");
}
EOC
pc_test
xgettext t.c </dev/null >o 2>e || pc_fail "xgettext: nonzero status"
[ -s messages.po ] || pc_fail "messages.po was not written"
[ "$(ids messages.po)" = 'alpha one file beta ' ] || pc_fail "extracted msgids: $(ids messages.po)"
grep -x 'msgid_plural "many files"' messages.po >/dev/null || pc_fail "no msgid_plural for the ngettext call"
grep -F 'msgstr[0] ""' messages.po >/dev/null || pc_fail "no empty msgstr[0] for the plural entry"
grep -x 'msgstr ""' messages.po >/dev/null || pc_fail "no empty msgstr for a singular entry"
grep -A1 -x 'msgid "one file"' messages.po | grep -x 'msgid_plural "many files"' >/dev/null || pc_fail "msgid_plural does not directly follow its msgid"
if command -v msgfmt >/dev/null 2>&1; then
	msgfmt -o x.mo messages.po </dev/null >o 2>e || pc_fail "msgfmt rejects the xgettext output"
fi
pc_pass
