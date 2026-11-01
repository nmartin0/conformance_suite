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
printf 'one\n' >f1; printf 'two\n' >f2; printf 'three\n' >f3; printf 'odd' >f4
touch -t 200001010000 f1 f2 f3 f4 || pc_unresolved "setup: touch failed"
touch -t 200001020000 ref || pc_unresolved "setup: touch failed"
ar -rc a f3 f1 f2 f4 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: cannot build the archive"
cat a >a.keep || pc_unresolved "setup: cannot copy the archive"
for f in f1 f2 f3 f4; do cp "$f" "$f.keep" || pc_unresolved "setup: cp failed"; done
rm -f f1 f2 f3 f4
pc_test
ar -x a f2 >out 2>err </dev/null || pc_fail "ar -x a f2: nonzero status"
[ -f f2 ] || pc_fail "f2 was not extracted"
cmp -s f2 f2.keep || pc_fail "f2 has the wrong contents"
[ -e f1 ] || [ -e f3 ] || [ -e f4 ] && pc_fail "ar -x a f2 extracted other members"
touch post || pc_unresolved "touch failed"
[ -e f2 ] && [ "$(find f2 -newer ref -print)" = f2 ] || pc_fail "f2 extracted by name does not carry the extraction time"
[ -z "$(find f2 -newer post -print)" ] || pc_fail "f2 carries a time later than the extraction"
cmp -s a a.keep || pc_fail "the archive was changed by extracting one member"
ar -x a >out 2>err </dev/null || pc_fail "ar -x a: nonzero status"
touch post2 || pc_unresolved "touch failed"
for f in f1 f2 f3 f4; do
	[ -f "$f" ] || pc_fail "ar -x a did not extract $f"
	cmp -s "$f" "$f.keep" || pc_fail "$f has the wrong contents after ar -x a"
	[ "$(find "$f" -newer ref -print)" = "$f" ] || pc_fail "$f does not carry the extraction time"
done
[ -z "$(find f1 f2 f3 f4 -newer post2 -print)" ] || pc_fail "an extracted file carries a time later than the extraction"
cmp -s a a.keep || pc_fail "the archive was changed by extraction"
pc_pass
