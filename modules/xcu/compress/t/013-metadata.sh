# compress keeps mode and modification time
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.compress.013
# MUTANT    compress.c_mode_600
# MUTANT    compress.c_mode_644
# MUTANT    compress.c_mtime_now
# MUTANT    compress.c_mtime_epoch
# MUTANT    compress.c_mtime_later
# MUTANT    compress.c_mtime_earlier
# MUTANT    compress.d_mode_600
# MUTANT    compress.d_mtime_now
# MUTANT    compress.d_mtime_epoch
# After "compress f" on a file with mode 640 and an old modification time,
# f.Z has mode 640 and that same modification time; after "compress -d f.Z"
# the restored f again has mode 640 and that same modification time.
. "$PC_CORE/pc.sh"
pc_begin XCU.compress.013
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
i=0; while [ "$i" -lt 200 ]; do printf 'line %d of some repetitive test text\n' "$i"; i=$((i + 1)); done >f || pc_unresolved "setup: cannot write input"
chmod 640 f || pc_unresolved "setup: chmod failed"
touch -t 200001010000 f || pc_unresolved "setup: touch failed"
touch -t 200001010000 ref || pc_unresolved "setup: touch failed"
pc_test
same_mtime() {
	# same modification time as ref: not newer than it, and ref not newer
	[ -n "$(find "$1" ! -newer ref -print)" ] && [ -n "$(find ref ! -newer "$1" -print)" ]
}
compress f </dev/null >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exit status $rc"
perms=$(ls -l f.Z | cut -c1-10)
[ "$perms" = '-rw-r-----' ] || pc_fail "mode of f.Z is $perms, expected -rw-r-----"
same_mtime f.Z || pc_fail "f.Z does not carry the modification time of f"
compress -d f.Z </dev/null >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "compress -d: exit status $rc"
perms=$(ls -l f | cut -c1-10)
[ "$perms" = '-rw-r-----' ] || pc_fail "mode of the restored f is $perms, expected -rw-r-----"
same_mtime f || pc_fail "the restored f does not carry the modification time of f.Z"
pc_pass
