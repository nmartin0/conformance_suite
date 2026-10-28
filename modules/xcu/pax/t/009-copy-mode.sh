# pax -rw copies a hierarchy
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.009
# "pax -rw d dest" copies directory d with its contents into the existing
# directory dest as dest/d.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.009
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mkdir d dest; printf 'nested\n' >d/f2
pc_test
pax -rw d dest >o 2>err </dev/null || pc_fail "pax -rw: nonzero status"
[ -d dest/d ] || pc_fail "dest/d was not created"
[ "$(cat dest/d/f2)" = nested ] || pc_fail "dest/d/f2 was not copied"
pc_pass
