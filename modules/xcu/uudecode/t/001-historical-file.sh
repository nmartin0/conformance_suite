# uudecode creates the named file with the mode from the data
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uudecode.001
# Decoding historical-format data whose begin line gives mode 640 and a name
# creates that file holding the decoded bytes, with permission bits
# rw-r----- even when the file mode creation mask is 077.
. "$PC_CORE/pc.sh"
pc_begin XCU.uudecode.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'begin 640 out.bin\n#0V%%T\n`\nend\n' >in || pc_unresolved "setup: cannot write input"
pc_test
umask 077
uudecode in >stdout 2>err
rc=$?
umask 022
[ "$rc" -eq 0 ] || pc_fail "exit status $rc"
[ -f out.bin ] || pc_fail "out.bin was not created"
[ "$(cat out.bin)" = Cat ] || pc_fail "decoded content is wrong"
perms=$(ls -l out.bin | cut -c1-10)
[ "$perms" = '-rw-r-----' ] || pc_fail "mode of the new file is $perms, expected -rw-r-----"
pc_pass
