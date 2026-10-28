# If the destination is the very directory entry of the source, ln diagnoses it and does not remove the file, even with -f.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ln.006
. "$PC_CORE/pc.sh"
pc_begin XCU.ln.006
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

printf 'keep\n' >a || pc_fail "cannot create a"
pc_test
ln -f a a >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "ln -f a a exited 0"
[ -s err ] || pc_fail "no diagnostic"
slurp a
[ "$data" = "keep${nl}" ] || pc_fail "a was removed or changed"
ln -f a ./a >out 2>err </dev/null
slurp a
[ "$data" = "keep${nl}" ] || pc_fail "a was removed or changed by ln -f a ./a"
pc_pass
