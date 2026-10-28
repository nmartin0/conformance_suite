# A prefix of PATH that does not end in a slash gets one inserted before the
# file name; a prefix that ends in a slash works as well, and the list is
# searched from beginning to end, so the first directory holding the file wins.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.8.3-other-environment-variables.006
#
. "$PC_CORE/pc.sh"
pc_begin XBD.8.3-other-environment-variables.006
d=$(pwd -P) || pc_unresolved "cannot find the working directory"
case $d in *:*|*%*) pc_unresolved "the working directory name holds a colon or a percent sign, which a PATH prefix cannot" ;; esac
mkdir b1 b2 || pc_unresolved "cannot make directories"
printf '%s\n' 'echo from-b1' >b1/pcprog || pc_unresolved "cannot write b1/pcprog"
printf '%s\n' 'echo from-b2' >b2/pcprog || pc_unresolved "cannot write b2/pcprog"
chmod 755 b1/pcprog b2/pcprog || pc_unresolved "chmod failed"
pc_test
out=$(PATH="$d/b1"; pcprog 2>/dev/null)
[ "$out" = from-b1 ] || pc_fail "PATH=$d/b1 (no trailing slash): pcprog gave \"$out\", not from-b1"
out=$(PATH="$d/b2/"; pcprog 2>/dev/null)
[ "$out" = from-b2 ] || pc_fail "PATH=$d/b2/ (trailing slash): pcprog gave \"$out\", not from-b2"
out=$(PATH="$d/nonexistent:$d/b2:$d/b1"; pcprog 2>/dev/null)
[ "$out" = from-b2 ] || pc_fail "PATH listing a missing directory, b2, b1: pcprog gave \"$out\", not from-b2"
out=$(PATH="$d/b1:$d/b2"; pcprog 2>/dev/null)
[ "$out" = from-b1 ] || pc_fail "PATH listing b1 then b2: pcprog gave \"$out\", not from-b1"
pc_pass
