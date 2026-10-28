# A file of the sought name without execute permission in an earlier PATH
# directory is passed over: the search goes on until an executable file is
# found.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.8.3-other-environment-variables.007
#
. "$PC_CORE/pc.sh"
pc_begin XBD.8.3-other-environment-variables.007
d=$(pwd -P) || pc_unresolved "cannot find the working directory"
case $d in *:*|*%*) pc_unresolved "the working directory name holds a colon or a percent sign, which a PATH prefix cannot" ;; esac
mkdir b1 b2 || pc_unresolved "cannot make directories"
printf '%s\n' 'echo from-b1' >b1/pcprog || pc_unresolved "cannot write b1/pcprog"
printf '%s\n' 'echo from-b2' >b2/pcprog || pc_unresolved "cannot write b2/pcprog"
chmod 644 b1/pcprog || pc_unresolved "chmod failed"
chmod 755 b2/pcprog || pc_unresolved "chmod failed"
# precondition: no execute permission on b1/pcprog for anyone, including the superuser
[ ! -x b1/pcprog ] || pc_notinuse "the file b1/pcprog without execute permission tests as executable"
pc_test
out=$(PATH="$d/b1:$d/b2"; pcprog 2>/dev/null)
[ "$out" = from-b2 ] || pc_fail "a non-executable pcprog in the first directory was not passed over: got \"$out\", not from-b2"
pc_pass
