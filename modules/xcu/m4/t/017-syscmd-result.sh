# syscmd result and streams
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.017
# The result of syscmd is not scanned for macros and m4 does not redirect the
# command's output.
. "$PC_CORE/pc.sh"
pc_begin XCU.m4.017
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >in <<'EOM'
define(`zz',`ZZ')syscmd(`echo zz')|syscmd(`echo err >&2')end
EOM
printf 'zz\n|end\n' >exp
pc_test
m4 in >out 2>err </dev/null || pc_fail "m4 failed"
cmp -s out exp || pc_fail "output was '$(cat out)', expected '$(cat exp)'"
grep err err >/dev/null || pc_fail "the command's standard error did not reach standard error"
pc_pass
