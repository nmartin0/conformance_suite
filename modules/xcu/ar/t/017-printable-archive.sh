# An archive of printable files is printable
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.017
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.017
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\n' >f1; printf 'three\n' >f2; printf 'odd length\n' >f3
ar -rc a f1 f2 f3 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: cannot build the archive"
[ -s a ] || pc_unresolved "setup: the archive is empty"
pc_test
n=$(tr -d '[:print:]\n\t' <a | wc -c)
[ "$n" -eq 0 ] || pc_fail "the archive contains $n characters that are not printable"
pc_pass
