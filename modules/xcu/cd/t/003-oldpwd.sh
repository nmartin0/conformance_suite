# OLDPWD holds the previous directory.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cd.003
# MUTANT    cd.oldpwd_unset
# MUTANT    cd.oldpwd_new
# MUTANT    cd.oldpwd_frozen
# VARIANT   cd.oldpwd_exported
# VARIANT   cd.oldpwd_unexported
. "$PC_CORE/pc.sh"
pc_begin XCU.cd.003
unset CDPATH
mkdir d1 d2 || pc_fail "cannot make directories"
base=$PWD
pc_test
cd d1 || pc_fail "cd d1 failed"
[ "${OLDPWD-unset}" = "$base" ] || pc_fail "OLDPWD is '${OLDPWD-unset}', wanted $base"
cd ../d2 || pc_fail "cd ../d2 failed"
[ "${OLDPWD-unset}" = "$base/d1" ] || pc_fail "OLDPWD is '${OLDPWD-unset}', wanted $base/d1"
pc_pass
