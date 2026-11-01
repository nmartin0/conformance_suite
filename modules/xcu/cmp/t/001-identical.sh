# For identical files cmp writes nothing and exits 0.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cmp.001
# MUTANT    cmp.ident_exit1
# MUTANT    cmp.ident_out
# MUTANT    cmp.ident_err
# MUTANT    cmp.empty_differ
# MUTANT    cmp.differ_exit0
# MUTANT    cmp.cap4096
. "$PC_CORE/pc.sh"
pc_begin XCU.cmp.001
# mk N: N lines of 100 bytes and a newline, a file well beyond any
# small buffer.
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
printf 'abc\ndef\n' >f2 || pc_fail "cannot create f2"
: >e1 || pc_fail "cannot create e1"
: >e2 || pc_fail "cannot create e2"
mk 700 >big1 || pc_fail "cannot create big1"
mk 700 >big2 || pc_fail "cannot create big2"
{ mk 699; printf '%sX\n' "${blk%a}"; } >big3 || pc_fail "cannot create big3"
pc_test
cmp f1 f2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cmp exited with status $rc for identical files"
[ ! -s out ] || pc_fail "output written"
[ ! -s err ] || pc_fail "diagnostic written"
cmp e1 e2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cmp exited with status $rc for empty files"
[ ! -s out ] && [ ! -s err ] || pc_fail "output written for empty files"
cmp big1 big2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cmp exited with status $rc for identical large files"
[ ! -s out ] && [ ! -s err ] || pc_fail "output written for identical large files"
# the utility must tell different files apart, or the status above means nothing
printf 'abc\ndeX\n' >f3 || pc_unresolved "cannot create f3"
cmp f1 f3 >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp exited with status $rc for files that differ, wanted 1"
cmp big1 big3 >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp exited with status $rc for large files that differ at the end, wanted 1"
pc_pass
