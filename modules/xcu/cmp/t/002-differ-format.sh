# For files that differ cmp writes the names, the byte number and the line number of the first difference, both counted from 1, and exits 1.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cmp.002
# MUTANT    cmp.byte_from0
# MUTANT    cmp.line_from0
# MUTANT    cmp.byte_line_swapped
# MUTANT    cmp.names_swapped
# MUTANT    cmp.name_one
# MUTANT    cmp.word_byte
# MUTANT    cmp.differ_stderr
# MUTANT    cmp.differ_no_newline
# MUTANT    cmp.last_diff
# MUTANT    cmp.differ_exit0
# MUTANT    cmp.differ_exit2
# MUTANT    cmp.byte_mod64k
# MUTANT    cmp.cap4096
. "$PC_CORE/pc.sh"
pc_begin XCU.cmp.002
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
# mk N: N lines of 100 bytes and a newline, a file well beyond any small buffer.
blk=aaaaaaaaaa
blk=$blk$blk$blk$blk$blk$blk$blk$blk$blk$blk
mk() {
	mk_i=0
	while [ "$mk_i" -lt "$1" ]; do
		printf '%s\n' "$blk"
		mk_i=$((mk_i + 1))
	done
}

printf 'abc\ndef\n' >f1 || pc_fail "cannot create f1"
printf 'abc\ndXf\n' >f2 || pc_fail "cannot create f2"
printf 'x\n' >g1 || pc_fail "cannot create g1"
printf 'y\n' >g2 || pc_fail "cannot create g2"
printf 'a\nb\nc\nX\n' >h1 || pc_fail "cannot create h1"
printf 'a\nb\nc\nY\n' >h2 || pc_fail "cannot create h2"
# two differences, on different lines: the first one is reported
printf 'abc\ndef\nghi\n' >k1 || pc_fail "cannot create k1"
printf 'abc\ndXf\ngXi\n' >k2 || pc_fail "cannot create k2"
# a large pair that differs in its very last line, at byte 70699 of 70700
mk 700 >big1 || pc_fail "cannot create big1"
{ mk 699; printf '%sX\n' "${blk%a}"; } >big2 || pc_fail "cannot create big2"
pc_test
cmp f1 f2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp exited with status $rc, expected 1"
slurp out
[ "$data" = "f1 f2 differ: char 6, line 2${nl}" ] || pc_fail "output is: $data"
cmp g1 g2 >out 2>err </dev/null
slurp out
[ "$data" = "g1 g2 differ: char 1, line 1${nl}" ] || pc_fail "output for the first byte is: $data"
cmp h1 h2 >out 2>err </dev/null
slurp out
[ "$data" = "h1 h2 differ: char 7, line 4${nl}" ] || pc_fail "output for the last line is: $data"
cmp k1 k2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp exited with status $rc for files with two differences, expected 1"
slurp out
[ "$data" = "k1 k2 differ: char 6, line 2${nl}" ] || pc_fail "output for two differences is: $data"
cmp big1 big2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp exited with status $rc for large files, expected 1"
slurp out
[ "$data" = "big1 big2 differ: char 70699, line 700${nl}" ] || pc_fail "output for large files is: $data"
pc_pass
