# -o with comments, continuation lines and plurals
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.msgfmt.001
# A well-formed dot-po file with comments, continued strings, a plural
# header and plural entries compiles to the file named by -o with status 0.
. "$PC_CORE/pc.sh"
pc_begin XCU.msgfmt.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >a.po <<'EOP'
# translator comment
#: ref.c:12
#. extracted comment
msgid ""
msgstr ""
"Plural-Forms: nplurals=2; plural=n != 1;\n"

msgid "hello"
msgstr "bon"
"jour"

#, c-format
msgid "one file"
msgid_plural "%d files"
msgstr[0] "un fichier"
msgstr[1] "%d fichiers"
EOP
pc_test
msgfmt -o out.mo a.po </dev/null >o 2>e
rc=$?
[ "$rc" -eq 0 ] || pc_fail "msgfmt: exit status $rc"
[ -s out.mo ] || pc_fail "the output file out.mo is missing or empty"
pc_pass
