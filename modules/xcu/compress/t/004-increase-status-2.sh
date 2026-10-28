# compress exit status 2 when the output would be larger
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.compress.004
# "compress -g f" on a one-byte file, whose gzip form is longer than the
# input, exits with status 2 and leaves f in place and unchanged; with -f
# the file is compressed (status 0, f.gz exists).
. "$PC_CORE/pc.sh"
pc_begin XCU.compress.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'a' >f || pc_unresolved "setup: cannot write input"
pc_test
compress -g f </dev/null >out 2>err
rc=$?
[ "$rc" -eq 2 ] || pc_fail "compress -g of a one-byte file: exit status $rc, expected 2"
[ -f f ] || pc_fail "the input file is gone"
[ "$(cat f)" = a ] || pc_fail "the input file was changed"
compress -g -f f </dev/null >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "compress -g -f: exit status $rc"
[ -f f.gz ] || pc_fail "-f did not produce f.gz"
pc_pass
