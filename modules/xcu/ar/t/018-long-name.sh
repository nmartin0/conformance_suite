# A long member name is not truncated
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.018
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.018
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
base=0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ
name=${base}${base}${base}.txt
printf 'long\n' >"$name" 2>/dev/null || pc_notinuse "the file system does not allow a name of ${#name} bytes"
ar -rc a "$name" </dev/null >/dev/null 2>&1 || pc_unresolved "setup: cannot build the archive"
pc_test
ar -t a >out 2>err </dev/null || pc_fail "ar -t: nonzero status"
printf '%s\n' "$name" >exp
cmp out exp >/dev/null 2>&1 || pc_fail "the listing is not the full name: $(cat out)"
rm -f "$name"
ar -x a >out 2>err </dev/null || pc_fail "ar -x: nonzero status"
[ "$(cat "$name" 2>/dev/null)" = long ] || pc_fail "the member did not extract under its full name"
pc_pass
