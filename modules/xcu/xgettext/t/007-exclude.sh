# -x
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.xgettext.007
# Strings whose msgid appears in the -x file are not extracted.
. "$PC_CORE/pc.sh"
pc_begin XCU.xgettext.007
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
ids() { grep '^msgid "[^"]' "$1" | sed 's/^msgid "//; s/"$//' | tr '\n' ' '; }
cat >t.c <<'EOC'
void f(void) { gettext("alpha"); gettext("beta"); gettext("gamma"); }
EOC
cat >ex.po <<'EOP'
msgid "beta"
msgstr "ignored"

msgid "other"
msgstr "x"
EOP
pc_test
xgettext -x ex.po t.c </dev/null >o 2>e || pc_fail "xgettext -x: nonzero status"
[ "$(ids messages.po)" = 'alpha gamma ' ] || pc_fail "-x produced: $(ids messages.po)"
pc_pass
