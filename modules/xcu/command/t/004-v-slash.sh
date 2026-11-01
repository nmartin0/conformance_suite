# command -v with a slash in the name writes an absolute pathname.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.command.004
# MUTANT    command.slash_as_given
# MUTANT    command.slash_basename
# MUTANT    command.slash_dirname
# MUTANT    command.slash_no_newline
# MUTANT    command.slash_extra_line
# MUTANT    command.slash_status1
# VARIANT   command.slash_naive_join
#
# Three spellings: with a leading dot, without one, and already absolute.
# Output is read back with read (see 003).
. "$PC_CORE/pc.sh"
pc_begin XCU.command.004
mkdir bin || pc_fail "cannot make a directory"
: >bin/pccmd || pc_fail "cannot write a file"
chmod +x bin/pccmd || pc_fail "cannot make the file executable"
base=$PWD
pc_test
for name in ./bin/pccmd bin/pccmd "$base/bin/pccmd"; do
	command -v "$name" >out 2>/dev/null
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "command -v $name exited $rc"
	pc_need_file out "the output of command -v"
	{ IFS= read -r l1; r1=$?; IFS= read -r l2; r2=$?; } <out
	[ "$r1" -eq 0 ] || pc_fail "command -v $name wrote '$l1' without a final newline (or nothing)"
	[ "$r2" -ne 0 ] && [ -z "$l2" ] || pc_fail "command -v $name wrote more than one line"
	case $l1 in
	/*) ;;
	*) pc_fail "command -v $name wrote '$l1', not an absolute pathname" ;;
	esac
	[ "$l1" -ef "$base/bin/pccmd" ] || pc_fail "command -v $name wrote '$l1', which is not $base/bin/pccmd"
done
pc_pass
