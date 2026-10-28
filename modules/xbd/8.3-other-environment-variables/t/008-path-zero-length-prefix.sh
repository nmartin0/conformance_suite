# A zero-length prefix in PATH (leading colon, trailing colon, or two
# adjacent colons) stands for the working directory.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.8.3-other-environment-variables.008
#
. "$PC_CORE/pc.sh"
pc_begin XBD.8.3-other-environment-variables.008
d=$(pwd -P) || pc_unresolved "cannot find the working directory"
case $d in *:*|*%*) pc_unresolved "the working directory name holds a colon or a percent sign, which a PATH prefix cannot" ;; esac
mkdir w b2 b3 || pc_unresolved "cannot make directories"
printf '%s\n' 'echo from-cwd' >w/pcprog || pc_unresolved "cannot write w/pcprog"
printf '%s\n' 'echo from-b2' >b2/pcprog || pc_unresolved "cannot write b2/pcprog"
chmod 755 w/pcprog b2/pcprog || pc_unresolved "chmod failed"
cd w || pc_unresolved "cannot change to w"
pc_test
out=$(PATH=":$d/b3"; pcprog 2>/dev/null)
[ "$out" = from-cwd ] || pc_fail "leading colon: pcprog gave \"$out\", not the working directory's"
out=$(PATH="$d/b3:"; pcprog 2>/dev/null)
[ "$out" = from-cwd ] || pc_fail "trailing colon: pcprog gave \"$out\", not the working directory's"
out=$(PATH="$d/b3::$d/b2"; pcprog 2>/dev/null)
[ "$out" = from-cwd ] || pc_fail "adjacent colons before b2: pcprog gave \"$out\", not the working directory's"
out=$(PATH="$d/b2::"; pcprog 2>/dev/null)
[ "$out" = from-b2 ] || pc_fail "a non-empty prefix before an empty one: pcprog gave \"$out\", not from-b2"
pc_pass
