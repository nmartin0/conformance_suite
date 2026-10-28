# ar -x extracts members
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.004
# "ar -x a f2" recreates f2 with its contents and leaves other members
# unextracted; "ar -x a" with no operand extracts all members; the archive
# file is unchanged; the extracted file's modification time is the time of
# extraction, not the old time of the original.
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\n' >f1; printf 'two\n' >f2; printf 'three\n' >f3
touch -t 200001010000 f1 f2 f3 || pc_unresolved "setup: touch failed"
touch -t 200001020000 ref || pc_unresolved "setup: touch failed"
ar -rc a f1 f2 f3 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: cannot build the archive"
cat a >a.keep || pc_unresolved "setup: cannot copy the archive"
rm -f f1 f2 f3
pc_test
ar -x a f2 >out 2>err </dev/null || pc_fail "ar -x a f2: nonzero status"
[ -f f2 ] || pc_fail "f2 was not extracted"
[ "$(cat f2)" = two ] || pc_fail "f2 has the wrong contents"
[ -e f1 ] || [ -e f3 ] && pc_fail "ar -x a f2 extracted other members"
ar -x a >out 2>err </dev/null || pc_fail "ar -x a: nonzero status"
[ "$(cat f1)" = one ] && [ "$(cat f3)" = three ] || pc_fail "ar -x a did not extract all members"
cmp -s a a.keep || pc_fail "the archive was changed by extraction"
new=$(find f1 f2 f3 -newer ref -print)
[ "$(printf '%s\n' "$new" | wc -l)" -eq 3 ] || pc_fail "an extracted file does not carry the extraction time"
pc_pass
