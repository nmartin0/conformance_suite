# The limits of the System Interfaces apply to utilities: getconf reports PATH_MAX of at least 256 and NAME_MAX of at least 14, and pathchk accepts a path of 239 bytes made of components of 14 bytes
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.2-utility-limits.006
. "$PC_CORE/pc.sh"
pc_begin XCU.1.2-utility-limits.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
pc_test
p=$(getconf PATH_MAX . 2>/dev/null) || pc_fail "getconf PATH_MAX failed"
case $p in undefined) ;; '' | *[!0-9]*) pc_fail "PATH_MAX is '$p'" ;; esac
[ "$p" = undefined ] || [ "$p" -ge 256 ] || pc_fail "PATH_MAX is $p"
n=$(getconf NAME_MAX . 2>/dev/null) || pc_fail "getconf NAME_MAX failed"
case $n in undefined) ;; '' | *[!0-9]*) pc_fail "NAME_MAX is '$n'" ;; esac
[ "$n" = undefined ] || [ "$n" -ge 14 ] || pc_fail "NAME_MAX is $n"
long=$(awk 'BEGIN { for (i = 0; i < 16; i++) printf "%sabcdefghijklmn", (i ? "/" : "") }')
[ "${#long}" = 239 ] || pc_unresolved "path has length ${#long}"
pathchk "$long" || pc_fail "pathchk rejected a path of 239 bytes"
pathchk -p "$long" || pc_fail "pathchk -p rejected a path of 239 bytes"
pc_pass
