# -c -v abnormality detection
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.msgfmt.005
# With -c and -v a newline mismatch or a c-format conversion mismatch gives
# a non-zero status, a clean file gives zero, and no-c-format suppresses the
# conversion check.
. "$PC_CORE/pc.sh"
pc_begin XCU.msgfmt.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >nl.po <<'EOP'
msgid "line\n"
msgstr "ligne"
EOP
cat >cf.po <<'EOP'
#, c-format
msgid "%d items"
msgstr "articles"
EOP
cat >ty.po <<'EOP'
#, c-format
msgid "%d items"
msgstr "%s articles"
EOP
cat >ok.po <<'EOP'
#, c-format
msgid "%d items\n"
msgstr "%d articles\n"
EOP
cat >nocf.po <<'EOP'
#, no-c-format
msgid "100% sure"
msgstr "certain"
EOP
pc_test
msgfmt -c -v -o o1.mo nl.po </dev/null >o 2>e
[ "$?" -ne 0 ] || pc_fail "a newline mismatch was not diagnosed"
msgfmt -c -v -o o2.mo cf.po </dev/null >o 2>e
[ "$?" -ne 0 ] || pc_fail "a c-format conversion count mismatch was not diagnosed"
msgfmt -c -v -o o3.mo ty.po </dev/null >o 2>e
[ "$?" -ne 0 ] || pc_fail "a c-format conversion type mismatch was not diagnosed"
msgfmt -c -v -o o4.mo ok.po </dev/null >o 2>e
[ "$?" -eq 0 ] || pc_fail "a consistent entry was diagnosed"
msgfmt -c -v -o o5.mo nocf.po </dev/null >o 2>e
[ "$?" -eq 0 ] || pc_fail "no-c-format did not suppress the conversion check"
pc_pass
