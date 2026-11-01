# ls -g writes the long format without the owner.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.014
# MUTANT    ls.g_is_l
# MUTANT    ls.g_drops_both
# MUTANT    ls.g_is_o
# MUTANT    ls.g_short
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.014
LC_ALL=POSIX; export LC_ALL
mkdir d || pc_fail "cannot create d"
: >d/f || pc_fail "cannot create d/f"
# Give the file a group that is not the owner's, where the privilege allows
# it, so that the owner and group fields differ; failure is ignored.
chgrp "$(( $(id -g) + 54321 ))" d/f 2>/dev/null
set -- $(ls -l d/f) || pc_fail "ls -l failed"
[ "$#" -eq 9 ] || pc_unresolved "setup: unexpected number of fields in ls -l"
lgrp=$4
pc_test
ls -g d/f >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -g exited with status $rc"
set -- $(cat out)
[ "$#" -eq 8 ] || pc_fail "expected 8 fields, found $#"
[ "$3" = "$lgrp" ] || pc_fail "third field is '$3', expected the group '$lgrp'"
[ "$8" = d/f ] || pc_fail "last field is '$8'"
pc_pass
