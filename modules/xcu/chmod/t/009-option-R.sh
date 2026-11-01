# chmod -R changes the mode of a directory operand and of every file below it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chmod.009
# MUTANT    chmod.ok_status1
# MUTANT    chmod.stdout_noise
# MUTANT    chmod.R_ignored
# MUTANT    chmod.R_depth1
# MUTANT    chmod.R_dirs_skipped
# MUTANT    chmod.R_files_skipped
# MUTANT    chmod.R_dotfiles_skipped
# MUTANT    chmod.R_first_entry
# MUTANT    chmod.R_last_skipped
# MUTANT    chmod.operand_first_only
# MUTANT    chmod.operand_last_only
# MUTANT    chmod.R_file_operand_skipped
# MUTANT    chmod.R_mode_from_top
# VARIANT   chmod.reimpl
# VARIANT   chmod.ls_flag
# VARIANT   chmod.dir_setgid
# VARIANT   chmod.R_dirs_last
#
# The mode is relative and the tree mixes files and directories, so every
# entry must be worked out from its own mode.
. "$PC_CORE/pc.sh"
pc_begin XCU.chmod.009
# The listing may add a flag after the mode, and a directory may carry the
# set-group-ID bit from its creation; both are left to the implementation.
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
	case $mo in
	d?????[sS]???)
		mo_a=${mo%"${mo#??????}"}
		mo_r=${mo#???????}
		mo_c=${mo%"$mo_r"}
		mo_c=${mo_c#??????}
		case $mo_c in s) mo_c=x ;; S) mo_c=- ;; esac
		mo=$mo_a$mo_c$mo_r
		;;
	esac
}
# run ARGS: chmod must succeed and write nothing to standard output.
run() {
	chmod "$@" >out 2>err </dev/null
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "chmod $* exited with status $rc"
	[ ! -s out ] || pc_fail "chmod $* wrote to standard output"
}
# want FILE MODE LABEL
want() {
	modeof "$1" || pc_fail "cannot list $1"
	[ "$mo" = "$2" ] || pc_fail "$3: $1 has mode $mo, expected $2"
}

mkdir t t/sub t/sub/deep t/sub2 u || pc_fail "cannot create directories"
for p in t/f t/.hid t/sub/g t/sub/.h2 t/sub/deep/g2 t/sub2/k u/w z; do
	: >"$p" || pc_fail "cannot create $p"
	chmod 644 "$p" || pc_fail "cannot chmod $p"
done
chmod 755 t t/sub t/sub/deep t/sub2 u || pc_fail "cannot chmod"
pc_test
run -R go-rwx t u z
for p in t t/sub t/sub/deep t/sub2 u; do
	want "$p" drwx------ "-R go-rwx"
done
for p in t/f t/.hid t/sub/g t/sub/.h2 t/sub/deep/g2 t/sub2/k u/w z; do
	want "$p" -rw------- "-R go-rwx"
done
pc_pass
