# pax -u in write mode with -a, and in copy mode
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.041
# "pax -w -a -u" appends a file newer than its archive member and leaves the
# archive alone for an older file; "pax -rw -u" replaces the destination
# file when the source is newer and keeps it when the source is older.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.041
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
count() { pax <"$1" 2>/dev/null | grep -c '^f1$'; }
# write mode: a newer file is appended
printf 'old\n' >f1 && touch -t 200101010000 f1 || pc_unresolved "setup: cannot make f1"
pax -w -f A f1 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: pax -w failed"
printf 'new\n' >f1 && touch -t 200201010000 f1 || pc_unresolved "setup: cannot update f1"
# write mode: an older file is not appended
pax -w -f B f1 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: pax -w failed"
# copy mode: sources of three ages
mkdir src dest || pc_unresolved "setup: mkdir failed"
printf 'first\n' >src/f1 && touch -t 200101010000 src/f1 || pc_unresolved "setup: cannot make src/f1"
pax -rw src dest </dev/null >/dev/null 2>&1 || pc_unresolved "setup: pax -rw failed"
pc_test
pax -w -a -u -f A f1 </dev/null >/dev/null 2>err || pc_fail "pax -w -a -u with a newer file: nonzero status"
[ "$(count A)" -eq 2 ] || pc_fail "after appending a newer f1 the archive lists f1 $(count A) times, expected 2"
mkdir x && (cd x && pax -r <../A >/dev/null 2>&1)
[ "$(cat x/f1 2>/dev/null)" = new ] || pc_fail "extracting the archive does not give the newer contents of f1"
printf 'older\n' >f1 && touch -t 200001010000 f1 || pc_unresolved "setup: cannot age f1"
pax -w -a -u -f B f1 </dev/null >/dev/null 2>err || pc_fail "pax -w -a -u with an older file: nonzero status"
[ "$(count B)" -eq 1 ] || pc_fail "an older f1 was appended: the archive lists f1 $(count B) times, expected 1"
printf 'second\n' >src/f1 && touch -t 200201010000 src/f1 || pc_unresolved "setup: cannot update src/f1"
pax -rw -u src dest </dev/null >/dev/null 2>err || pc_fail "pax -rw -u with a newer source: nonzero status"
[ "$(cat dest/src/f1 2>/dev/null)" = second ] || pc_fail "a newer source did not replace dest/src/f1: '$(cat dest/src/f1 2>/dev/null)'"
printf 'third\n' >src/f1 && touch -t 200001010000 src/f1 || pc_unresolved "setup: cannot age src/f1"
pax -rw -u src dest </dev/null >/dev/null 2>err || pc_fail "pax -rw -u with an older source: nonzero status"
[ "$(cat dest/src/f1 2>/dev/null)" = second ] || pc_fail "an older source replaced dest/src/f1: '$(cat dest/src/f1 2>/dev/null)'"
pc_pass
