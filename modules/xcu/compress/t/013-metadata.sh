# compress keeps mode and modification time
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.compress.013
# After "compress f" on a file with mode 640 and an old modification time,
# f.Z has mode 640 and a modification time no later than that old time.
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
touch -t 200001020000 ref || pc_unresolved "setup: touch failed"
pc_test
compress f </dev/null >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exit status $rc"
perms=$(ls -l f.Z | cut -c1-10)
[ "$perms" = '-rw-r-----' ] || pc_fail "mode of f.Z is $perms, expected -rw-r-----"
old=$(find f.Z ! -newer ref -print)
[ -n "$old" ] || pc_fail "f.Z does not carry the old modification time of f"
pc_pass
