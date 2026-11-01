# ls -o writes the long format without the group.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.015
# MUTANT    ls.o_is_l
# MUTANT    ls.o_drops_both
# MUTANT    ls.o_is_g
# MUTANT    ls.o_short
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.015
LC_ALL=POSIX; export LC_ALL
mkdir d || pc_fail "cannot create d"
: >d/f || pc_fail "cannot create d/f"
# Give the file a group that is not the owner's, where the privilege allows
# it, so that the owner and group fields differ; failure is ignored.
chgrp "$(( $(id -g) + 54321 ))" d/f 2>/dev/null
set -- $(ls -l d/f) || pc_fail "ls -l failed"
[ "$#" -eq 9 ] || pc_unresolved "setup: unexpected number of fields in ls -l"
lown=$3
pc_test
ls -o d/f >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -o exited with status $rc"
set -- $(cat out)
[ "$#" -eq 8 ] || pc_fail "expected 8 fields, found $#"
[ "$3" = "$lown" ] || pc_fail "third field is '$3', expected the owner '$lown'"
[ "$8" = d/f ] || pc_fail "last field is '$8'"
pc_pass
