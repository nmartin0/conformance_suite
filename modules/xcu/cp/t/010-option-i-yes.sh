# With -i, an affirmative reply makes cp overwrite the existing destination.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.010
# MUTANT    cp.ok_status1
# MUTANT    cp.i_no_prompt
# MUTANT    cp.i_prompt_no_name
# MUTANT    cp.i_prompt_stdout
# MUTANT    cp.i_yes_ignored
# MUTANT    cp.i_yes_status1
# VARIANT    cp.prompt_nl
# VARIANT    cp.prompt_quoted
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.010
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

printf 'new\n' >src || pc_fail "cannot create src"
printf 'old\n' >dstfile || pc_fail "cannot create dstfile"
pc_test
printf 'y\n' | cp -i src dstfile >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cp -i exited with status $rc after an affirmative reply"
slurp dstfile
[ "$data" = "new${nl}" ] || pc_fail "destination was not overwritten"
slurp err
case $data in
*dstfile*) ;;
*) pc_fail "no prompt naming the destination on standard error" ;;
esac
pc_pass
