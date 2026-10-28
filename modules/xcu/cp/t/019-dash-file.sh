# The operand - names a file called -, for both source and target; it never means a standard stream.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.019
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.019
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

printf 'dash\n' >./- || pc_fail "cannot create file named -"
printf 'plain\n' >src || pc_fail "cannot create src"
pc_test
cp - copy >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cp - copy exited with status $rc"
slurp copy
[ "$data" = "dash${nl}" ] || pc_fail "source - was not the file named -"
cp src - >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cp src - exited with status $rc"
slurp ./-
[ "$data" = "plain${nl}" ] || pc_fail "target - was not the file named -"
[ ! -s out ] || pc_fail "cp wrote to standard output"
pc_pass
