# cmp compares bytes of any value, null bytes included, and accepts a character special file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cmp.009
# MUTANT    cmp.bin_nul_stops
# MUTANT    cmp.bin_high_masked
# MUTANT    cmp.bin_special_rejected
# MUTANT    cmp.bin_partial_line_ignored
. "$PC_CORE/pc.sh"
pc_begin XCU.cmp.009
nl='
'
slurp() {
	data=
	l=
	while IFS= read -r l; do
		data=$data$l$nl
		l=
	done <"$1"
	[ -z "$l" ] || data=$data$l'<noeol>'
	return 0
}

printf 'a\000b' >f1 || pc_fail "cannot create f1"
printf 'a\000c' >f2 || pc_fail "cannot create f2"
printf 'a\000b' >f3 || pc_fail "cannot create f3"
: >e || pc_fail "cannot create e"
printf 'a\nb' >p1 || pc_fail "cannot create p1"
printf 'a\nc' >p2 || pc_fail "cannot create p2"
printf '\200' >h1 || pc_fail "cannot create h1"
printf '\000' >h2 || pc_fail "cannot create h2"
pc_test
cmp f1 f3 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cmp exited with status $rc for equal files with a null byte"
cmp f1 f2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp exited with status $rc for files differing after a null byte"
slurp out
[ "$data" = "f1 f2 differ: char 3, line 1${nl}" ] || pc_fail "output is: $data"
cmp e /dev/null >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cmp e /dev/null exited with status $rc"
cmp f1 /dev/null >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp f1 /dev/null exited with status $rc, expected 1"
# the last line has no newline
cmp p1 p2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp exited with status $rc for files differing in an unterminated last line"
slurp out
[ "$data" = "p1 p2 differ: char 3, line 2${nl}" ] || pc_fail "output for an unterminated last line is: $data"
# a byte with the high bit set differs from one without
cmp h1 h2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp exited with status $rc for bytes 128 and 0"
slurp out
[ "$data" = "h1 h2 differ: char 1, line 1${nl}" ] || pc_fail "output for bytes 128 and 0 is: $data"
pc_pass
