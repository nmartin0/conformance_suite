# touch creates a missing file as an empty regular file with mode rw-rw-rw- reduced by the creation mask, and leaves the contents of an existing file alone.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.touch.001
. "$PC_CORE/pc.sh"
pc_begin XCU.touch.001
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
}

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

printf 'keep\n' >old || pc_fail "cannot create old"
pc_test
(umask 022; touch new1 new2 old) >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "touch exited with status $rc"
[ -f new1 ] && [ -f new2 ] || pc_fail "a missing file was not created"
[ ! -s new1 ] || pc_fail "new1 is not empty"
modeof new1 || pc_fail "cannot list new1"
[ "$mo" = "-rw-r--r--" ] || pc_fail "umask 022: mode is $mo"
(umask 077; touch new3) >out 2>err </dev/null
modeof new3 || pc_fail "cannot list new3"
[ "$mo" = "-rw-------" ] || pc_fail "umask 077: mode is $mo"
slurp old
[ "$data" = "keep${nl}" ] || pc_fail "old file contents changed"
[ ! -s out ] || pc_fail "touch wrote to standard output"
pc_pass
