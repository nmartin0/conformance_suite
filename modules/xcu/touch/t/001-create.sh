# touch creates a missing file as an empty regular file with mode rw-rw-rw- reduced by the creation mask, and leaves the contents and the mode of an existing file alone.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.touch.001
# MUTANT    touch.ok_status1
# MUTANT    touch.stdout_noise
# MUTANT    touch.create_ignores_umask
# MUTANT    touch.create_mode_644
# MUTANT    touch.create_mode_exec
# MUTANT    touch.create_nonempty
# MUTANT    touch.create_truncates
# MUTANT    touch.create_only_first
# MUTANT    touch.create_dir
# MUTANT    touch.create_nothing
# MUTANT    touch.chmod_existing
# MUTANT    touch.c_inverted
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
chmod 640 old || pc_fail "cannot set the mode of old"
pc_test
(umask 022; touch new1 new2 old) >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "touch exited with status $rc"
[ -f new1 ] && [ -f new2 ] || pc_fail "a missing file was not created"
[ ! -s new1 ] || pc_fail "new1 is not empty"
[ ! -s new2 ] || pc_fail "new2 is not empty"
modeof new1 || pc_fail "cannot list new1"
[ "$mo" = "-rw-r--r--" ] || pc_fail "umask 022: mode is $mo"
modeof new2 || pc_fail "cannot list new2"
[ "$mo" = "-rw-r--r--" ] || pc_fail "umask 022: mode of the second file is $mo"
(umask 077; touch new3) >out3 2>err3 </dev/null
[ -f new3 ] || pc_fail "new3 was not created"
modeof new3 || pc_fail "cannot list new3"
[ "$mo" = "-rw-------" ] || pc_fail "umask 077: mode is $mo"
(umask 000; touch new4) >out4 2>err4 </dev/null
[ -f new4 ] || pc_fail "new4 was not created"
modeof new4 || pc_fail "cannot list new4"
[ "$mo" = "-rw-rw-rw-" ] || pc_fail "umask 000: mode is $mo"
modeof old || pc_fail "cannot list old"
[ "$mo" = "-rw-r-----" ] || pc_fail "the mode of the existing file changed to $mo"
slurp old
[ "$data" = "keep${nl}" ] || pc_fail "old file contents changed"
[ ! -s out ] || pc_fail "touch wrote to standard output"
pc_pass
