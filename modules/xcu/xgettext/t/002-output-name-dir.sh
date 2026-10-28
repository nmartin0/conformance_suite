# -d and -p
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.xgettext.002
# -d name writes name.po instead of messages.po, and -p dir writes the file
# into dir.
. "$PC_CORE/pc.sh"
pc_begin XCU.xgettext.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
ids() { grep '^msgid "[^"]' "$1" | sed 's/^msgid "//; s/"$//' | tr '\n' ' '; }
cat >t.c <<'EOC'
void f(void) { gettext("alpha"); }
EOC
mkdir out || pc_unresolved "setup: cannot make a directory"
pc_test
xgettext -d mydom t.c </dev/null >o 2>e || pc_fail "xgettext -d: nonzero status"
[ -s mydom.po ] || pc_fail "-d mydom did not write mydom.po"
[ -e messages.po ] && pc_fail "-d mydom still wrote messages.po"
xgettext -p out t.c </dev/null >o 2>e || pc_fail "xgettext -p: nonzero status"
[ -s out/messages.po ] || pc_fail "-p out did not write out/messages.po"
[ -e messages.po ] && pc_fail "-p out also wrote messages.po in the current directory"
pc_pass
