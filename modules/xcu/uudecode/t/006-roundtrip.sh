# uuencode then uudecode restores the bytes
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uudecode.006
# For inputs of 0, 1, 2, 3, 44, 45, 46 and 100 bytes holding non-text bytes,
# uuencode piped to uudecode (both algorithms) gives back exactly the
# original bytes.
. "$PC_CORE/pc.sh"
pc_begin XCU.uudecode.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
{ printf '\000\001\002\177\200\377\n\r\t '; printf '%0111d' 0; } >base || pc_unresolved "setup: cannot write input"
pc_test
for n in 0 1 2 3 44 45 46 100; do
	dd if=base of=orig bs=1 count=$n 2>/dev/null && chmod 644 orig || pc_unresolved "cannot cut the input"
	uuencode orig out.dat >enc 2>err || pc_fail "uuencode failed for $n bytes"
	rm -f out.dat
	uudecode enc >stdout 2>err || pc_fail "uudecode failed for $n bytes"
	cmp -s orig out.dat || pc_fail "bytes differ for $n bytes"
	uuencode -m orig out.dat >enc 2>err || pc_fail "uuencode -m failed for $n bytes"
	rm -f out.dat
	uudecode enc >stdout 2>err || pc_fail "uudecode failed for -m $n bytes"
	cmp -s orig out.dat || pc_fail "bytes differ for -m $n bytes"
done
pc_pass
