# nm of a file without symbols
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nm.009
# An object file with no symbolic information is not an error: nm exits 0.
. "$PC_CORE/pc.sh"
pc_begin XCU.nm.009
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
printf 'typedef int pc_t;\n' >e.c
c17 -c e.c </dev/null >/dev/null 2>&1 || pc_unresolved "setup: c17 -c failed"
[ -f e.o ] || pc_unresolved "setup: no e.o"
pc_test
nm e.o >o 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "nm of an object with no symbols: exit status $rc"
pc_pass
