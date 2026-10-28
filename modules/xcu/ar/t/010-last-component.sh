# members are named by the last pathname component
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.010
# A file added as sub/f3 is stored under the name f3: "ar -t a" lists f3,
# and a later "ar -d a other/f3" (same last component) removes it.
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.010
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mkdir sub other || pc_unresolved "setup: mkdir failed"
printf 'one\n' >f1; printf 'three\n' >sub/f3
pc_test
ar -rc a f1 sub/f3 >out 2>err </dev/null || pc_fail "ar -r with a path operand: nonzero status"
[ "$(ar -t a </dev/null)" = "$(printf 'f1\nf3')" ] || pc_fail "the member is not stored under the last component"
ar -d a other/f3 >out 2>err </dev/null || pc_fail "ar -d with a different directory: nonzero status"
[ "$(ar -t a </dev/null)" = f1 ] || pc_fail "ar -d did not match the last component"
pc_pass
