# -D
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.msgfmt.004
# -D dir makes msgfmt find an input file in that directory.
. "$PC_CORE/pc.sh"
pc_begin XCU.msgfmt.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mkdir sub || pc_unresolved "setup: cannot make a directory"
cat >sub/a.po <<'EOP'
msgid "one"
msgstr "un"
EOP
pc_test
msgfmt -D sub -o out.mo a.po </dev/null >o 2>e
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-D sub did not find sub/a.po (status $rc)"
[ -s out.mo ] || pc_fail "no output file was written"
pc_pass
