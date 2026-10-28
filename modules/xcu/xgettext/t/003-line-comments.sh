# -n
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.xgettext.003
# -n adds a #: comment with pathname:line before the msgid directive.
. "$PC_CORE/pc.sh"
pc_begin XCU.xgettext.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
ids() { grep '^msgid "[^"]' "$1" | sed 's/^msgid "//; s/"$//' | tr '\n' ' '; }
cat >t.c <<'EOC'
void f(void)
{
	int x;
	gettext("alpha");
}
EOC
pc_test
xgettext -n t.c </dev/null >o 2>e || pc_fail "xgettext -n: nonzero status"
awk '/^msgid /{ if ($0 == "msgid \"alpha\"" && ref) ok = 1; ref = 0 } /^#:/{ ref = 1 } END { exit !ok }' messages.po || pc_fail "no #: comment line before the msgid"
grep '^#:.*t\.c:4' messages.po >/dev/null || pc_fail "no t.c:4 reference: $(grep '^#:' messages.po)"
pc_pass
