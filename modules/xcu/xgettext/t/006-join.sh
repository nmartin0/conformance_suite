# -j
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.xgettext.006
# -j appends new messages to an existing dot-po file and does not repeat an
# msgid already present.
. "$PC_CORE/pc.sh"
pc_begin XCU.xgettext.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
ids() { grep '^msgid "[^"]' "$1" | sed 's/^msgid "//; s/"$//' | tr '\n' ' '; }
cat >t1.c <<'EOC'
void f(void) { gettext("alpha"); gettext("beta"); }
EOC
cat >t2.c <<'EOC'
void g(void) { gettext("beta"); gettext("gamma"); }
EOC
pc_test
xgettext t1.c </dev/null >o 2>e || pc_fail "first xgettext: nonzero status"
xgettext -j t2.c </dev/null >o 2>e || pc_fail "xgettext -j: nonzero status"
[ "$(ids messages.po)" = 'alpha beta gamma ' ] || pc_fail "-j produced: $(ids messages.po)"
rm -f messages.po
xgettext -j t1.c </dev/null >o 2>e || pc_fail "xgettext -j on a missing file: nonzero status"
[ "$(ids messages.po)" = 'alpha beta ' ] || pc_fail "-j did not create the dot-po file: $(ids messages.po)"
pc_pass
