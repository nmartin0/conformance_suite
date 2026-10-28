# uudecode overwrites an existing file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uudecode.007
# If the output file already exists and can be written, its old contents are
# replaced entirely by the decoded bytes.
. "$PC_CORE/pc.sh"
pc_begin XCU.uudecode.007
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'begin 644 o.out\n#0V%%T\n`\nend\n' >in || pc_unresolved "setup: cannot write input"
printf 'old contents that are longer\n' >o.out || pc_unresolved "setup: cannot write old file"
pc_test
uudecode in >stdout 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exit status $rc"
n=$(wc -c <o.out); n=$((n + 0))
[ "$n" -eq 3 ] || pc_fail "file has $n bytes after decoding, expected 3"
[ "$(cat o.out)" = Cat ] || pc_fail "decoded content is wrong"
pc_pass
