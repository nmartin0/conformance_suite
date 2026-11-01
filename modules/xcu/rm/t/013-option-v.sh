# rm -v writes a message containing the pathname of each file it removed to standard output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rm.013
# MUTANT    rm.v_none
# MUTANT    rm.v_stderr
# MUTANT    rm.v_no_name
# MUTANT    rm.v_first_only
# MUTANT    rm.v_top_only
# MUTANT    rm.v_names_failed
# VARIANT   rm.v_quoted
# VARIANT   rm.v_two_lines
# VARIANT   rm.reverse_order
# VARIANT   rm.ref
. "$PC_CORE/pc.sh"
pc_begin XCU.rm.013
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

printf x >vfile1 || pc_fail "cannot create vfile1"
printf x >vfile2 || pc_fail "cannot create vfile2"
pc_test
rm -v vfile1 vfile2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "rm -v exited with status $rc"
[ ! -e vfile1 ] && [ ! -e vfile2 ] || pc_fail "files were not removed"
slurp out
case $data in
*vfile1*) ;;
*) pc_fail "no message naming vfile1" ;;
esac
case $data in
*vfile2*) ;;
*) pc_fail "no message naming vfile2" ;;
esac
rm -v vnone >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "rm -v exited 0 for a missing operand"
slurp out
case $data in
*vnone*) pc_fail "a message reports a file that does not exist as removed" ;;
esac
mkdir -p vtree/vsub || pc_fail "cannot create vtree"
printf x >vtree/vtopfile || pc_fail "cannot create vtopfile"
printf x >vtree/vsub/vdeepfile || pc_fail "cannot create vdeepfile"
rm -v -r vtree >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "rm -v -r exited with status $rc"
[ ! -e vtree ] || pc_fail "vtree still exists"
slurp out
case $data in
*vtopfile*) ;;
*) pc_fail "no message naming vtopfile" ;;
esac
case $data in
*vdeepfile*) ;;
*) pc_fail "no message naming vdeepfile" ;;
esac
pc_pass
