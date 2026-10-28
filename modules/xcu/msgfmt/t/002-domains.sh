# domain directives and -o ignoring them
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.msgfmt.002
# With -S each domain section goes to domainname.mo and the leading section
# to messages.mo; with -o a single file is written and domain directives are
# ignored.
. "$PC_CORE/pc.sh"
pc_begin XCU.msgfmt.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >a.po <<'EOP'
msgid "one"
msgstr "un"

domain "foo"
msgid "two"
msgstr "deux"
EOP
pc_test
msgfmt -S a.po </dev/null >o 2>e || pc_fail "msgfmt -S: nonzero status"
[ -s foo.mo ] || pc_fail "domain foo did not produce foo.mo"
[ -s messages.mo ] || pc_fail "the leading section did not produce messages.mo"
rm -f foo.mo messages.mo
msgfmt -o only.mo a.po </dev/null >o 2>e || pc_fail "msgfmt -o: nonzero status"
[ -s only.mo ] || pc_fail "-o only.mo wrote no file"
[ -e foo.mo ] && pc_fail "-o did not ignore the domain directive (foo.mo exists)"
[ -e messages.mo ] && pc_fail "-o did not ignore the domain directive (messages.mo exists)"
[ -e foo ] && pc_fail "-o did not ignore the domain directive (foo exists)"
pc_pass
