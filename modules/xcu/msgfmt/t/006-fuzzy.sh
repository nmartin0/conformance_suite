# fuzzy entries and -f
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.msgfmt.006
# A fuzzy entry is left out of the output unless -f is given, so the outputs
# with and without -f differ and the one without matches a file with no such
# entry.
. "$PC_CORE/pc.sh"
pc_begin XCU.msgfmt.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >f.po <<'EOP'
msgid "keep"
msgstr "garde"

#, fuzzy
msgid "maybe"
msgstr "peutetre"
EOP
cat >nf.src <<'EOP'
msgid "keep"
msgstr "garde"
EOP
pc_test
msgfmt -o plain.mo f.po </dev/null >o 2>e || pc_fail "msgfmt (fuzzy input): nonzero status"
msgfmt -f -o withf.mo f.po </dev/null >o 2>e || pc_fail "msgfmt -f: nonzero status"
cp nf.src f.po || pc_unresolved "cannot replace the input"
msgfmt -o ref.mo f.po </dev/null >o 2>e || pc_fail "msgfmt (reference): nonzero status"
[ -s plain.mo ] && [ -s withf.mo ] && [ -s ref.mo ] || pc_fail "an output file is missing"
cmp plain.mo ref.mo >/dev/null 2>&1 || pc_fail "the fuzzy entry was included without -f"
cmp plain.mo withf.mo >/dev/null 2>&1 && pc_fail "-f made no difference"
pc_pass
