# With -p, an operand that names an existing directory is ignored without error or diagnostic.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mkdir.007
# MUTANT    mkdir.p_exist_error
# MUTANT    mkdir.p_exist_chmod
# MUTANT    mkdir.p_exist_clobber
. "$PC_CORE/pc.sh"
pc_begin XCU.mkdir.007
mkdir -m 700 d || pc_fail "cannot create d"
printf k >d/keep || pc_fail "cannot create d/keep"
pc_test
mkdir -p d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "mkdir -p d exited with status $rc"
mkdir -p d/e >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "mkdir -p d/e exited with status $rc"
[ -d d/e ] || pc_fail "d/e was not created"
mo=$(ls -ld d 2>/dev/null) || pc_fail "cannot list d"
mo=${mo%% *}
[ "${mo%"${mo#??????????}"}" = "drwx------" ] || pc_fail "existing d changed its mode to $mo"
[ -f d/keep ] || pc_fail "existing d lost its contents"
[ ! -s err ] || pc_fail "diagnostic written"
pc_pass
