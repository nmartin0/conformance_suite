# - as a file operand
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.xgettext.008
# A file operand of - reads the C source from standard input.
. "$PC_CORE/pc.sh"
pc_begin XCU.xgettext.008
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
ids() { grep '^msgid "[^"]' "$1" | sed 's/^msgid "//; s/"$//' | tr '\n' ' '; }
cat >t.c <<'EOC'
void f(void) { gettext("from stdin"); }
EOC
pc_test
xgettext - <t.c >o 2>e || pc_fail "xgettext -: nonzero status"
[ "$(ids messages.po)" = 'from stdin ' ] || pc_fail "standard input produced: $(ids messages.po)"
pc_pass
