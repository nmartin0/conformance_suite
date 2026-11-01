# When one file is a prefix of the other, cmp exits 1 and writes an EOF diagnostic naming the shorter file to standard error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cmp.005
# MUTANT    cmp.eof_exit0
# MUTANT    cmp.eof_exit2
# MUTANT    cmp.eof_no_diag
# MUTANT    cmp.eof_stdout
# MUTANT    cmp.eof_swap_name
# MUTANT    cmp.eof_no_prefix
# MUTANT    cmp.eof_no_nl
# MUTANT    cmp.eof_info_nospace
# MUTANT    cmp.eof_info_nl
# MUTANT    cmp.eof_both
# MUTANT    cmp.eof_empty_ok
# MUTANT    cmp.cap4096
# VARIANT   cmp.eof_noinfo
# VARIANT   cmp.eof_info_words
. "$PC_CORE/pc.sh"
pc_begin XCU.cmp.005
# The additional information after the name is null or starts with a blank
# and has no newline, so the diagnostic is one line and starts as below.
# chk FILE NAME: FILE holds one newline-terminated line, "cmp: EOF on NAME"
# alone or followed by a blank and more.
chk() {
	chk_e=
	chk_x=
	{
		IFS= read -r chk_e || pc_fail "the diagnostic is empty or lacks its newline"
		IFS= read -r chk_x
		[ -z "$chk_x" ] || pc_fail "the diagnostic has more than one line"
	} <"$1"
	case $chk_e in
	"cmp: EOF on $2" | "cmp: EOF on $2 "*) ;;
	*) pc_fail "diagnostic is: $chk_e" ;;
	esac
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
printf 'abc' >f1 || pc_fail "cannot create f1"
printf 'abcd' >f2 || pc_fail "cannot create f2"
: >e1 || pc_fail "cannot create e1"
mk 700 >big1 || pc_fail "cannot create big1"
{ mk 700; printf 'b'; } >big2 || pc_fail "cannot create big2"
pc_test
cmp f1 f2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp exited with status $rc, expected 1"
chk err f1
cmp f2 f1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp (reversed) exited with status $rc, expected 1"
chk err f1
# an empty file is the first part of every file
cmp e1 f1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp exited with status $rc for an empty first file, expected 1"
chk err e1
cmp f1 e1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp exited with status $rc for an empty second file, expected 1"
chk err e1
# a large file and the same file with one byte more
cmp big2 big1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp exited with status $rc for a large prefix, expected 1"
chk err big1
pc_pass
