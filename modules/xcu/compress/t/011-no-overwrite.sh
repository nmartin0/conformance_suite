# compress refuses to overwrite without -f
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.compress.011
# VARIANT   compress.lzw_other_format
# VARIANT   compress.refuse_status_big
# MUTANT    compress.overwrite_silently
# MUTANT    compress.overwrite_exit1
# MUTANT    compress.overwrite_exit0
# MUTANT    compress.refuse_no_diag
# MUTANT    compress.force_ignored
# MUTANT    compress.force_garbage
# MUTANT    compress.refuse_removes_input
# MUTANT    compress.refuse_truncates_old
# MUTANT    compress.refuse_old_appended
# With f.Z already present, standard input not a terminal and no -f,
# "compress f" writes a diagnostic to standard error, exits nonzero and
# leaves the existing f.Z unchanged; with -f the output file is replaced by
# valid compressed data.
. "$PC_CORE/pc.sh"
pc_begin XCU.compress.011
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
i=0; while [ "$i" -lt 200 ]; do printf 'line %d of some repetitive test text\n' "$i"; i=$((i + 1)); done >orig || pc_unresolved "setup: cannot write input"
cat orig >f || pc_unresolved "setup: cannot copy input"
printf 'keep' >f.Z || pc_unresolved "setup: cannot write old output"
pc_test
compress f </dev/null >out 2>err
rc=$?
[ "$rc" -ne 0 ] || pc_fail "overwriting f.Z without -f gave status 0"
[ -s err ] || pc_fail "no diagnostic on standard error for the refused overwrite"
[ "$(cat f.Z)" = keep ] || pc_fail "the existing f.Z was overwritten"
[ -f f ] || pc_fail "the input file is gone"
compress -f f </dev/null >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "compress -f: exit status $rc"
compress -d -c f.Z </dev/null >back 2>err
cmp -s back orig || pc_fail "f.Z written with -f does not decompress to the original"
pc_pass
