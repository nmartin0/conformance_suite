# -S
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.msgfmt.003
# -S appends .mo to a generated name when it is not already there and
# never doubles a .mo suffix (whether -S applies to a -o name is not stated,
# so a run that leaves the -o name alone is UNRESOLVED).
. "$PC_CORE/pc.sh"
pc_begin XCU.msgfmt.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >a.po <<'EOP'
msgid "one"
msgstr "un"
EOP
pc_test
msgfmt -S -o out a.po </dev/null >o 2>e || pc_fail "msgfmt -S -o out: nonzero status"
if [ ! -s out.mo ]; then
	[ -s out ] && pc_unresolved "-S with -o: the page does not say whether -S applies to the -o name"
	pc_fail "-S -o out wrote neither out.mo nor out"
fi
rm -f out out.mo
msgfmt -S -o named.mo a.po </dev/null >o 2>e || pc_fail "msgfmt -S -o named.mo: nonzero status"
[ -s named.mo ] || pc_fail "named.mo was not written"
[ -e named.mo.mo ] && pc_fail "-S appended a second .mo suffix"
pc_pass
