# command -v writes just the name for functions, special built-ins and reserved words.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.command.005
# MUTANT    command.names_fn_extra
# MUTANT    command.names_fn_path
# MUTANT    command.names_fn_silent
# MUTANT    command.names_fn_status1
# MUTANT    command.names_fn_no_newline
# MUTANT    command.names_special_path
# MUTANT    command.names_special_silent
# MUTANT    command.names_special_status1
# MUTANT    command.names_reserved_silent
# MUTANT    command.names_reserved_extra
# MUTANT    command.names_reserved_status1
#
# Output is read back with read, so the line must be the bare name with
# its newline and nothing else. Several names of each kind are tried.
. "$PC_CORE/pc.sh"
pc_begin XCU.command.005
pcfn() { :; }
pcfn2() { :; }
pc_chk() {
	command -v "$1" >out 2>/dev/null
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "command -v $1 ($2) exited $rc"
	pc_need_file out "the output of command -v"
	{ IFS= read -r l1; r1=$?; IFS= read -r l2; r2=$?; } <out
	[ "$r1" -eq 0 ] || pc_fail "command -v $1 ($2) wrote '$l1' without a final newline (or nothing)"
	[ "$r2" -ne 0 ] && [ -z "$l2" ] || pc_fail "command -v $1 ($2) wrote more than one line"
	[ "$l1" = "$1" ] || pc_fail "for $2 command -v wrote '$l1', wanted $1"
}
pc_test
pc_chk pcfn "a function"
pc_chk pcfn2 "a function"
pc_chk exec "a special built-in"
pc_chk : "a special built-in"
pc_chk eval "a special built-in"
pc_chk export "a special built-in"
pc_chk if "a reserved word"
pc_chk then "a reserved word"
pc_chk '!' "a reserved word"
pc_chk '{' "a reserved word"
pc_pass
