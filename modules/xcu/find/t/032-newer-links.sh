# find -newer with a symbolic-link argument
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.032
. "$PC_CORE/pc.sh"
pc_begin XCU.find.032
printf 't\n' >target || pc_unresolved "cannot create target"
touch -d 2003-01-02T15:04:05Z target || pc_unresolved "cannot set the time of target"
ln -s target link || pc_unresolved "cannot create symbolic link"
ln -s nowhere dang || pc_unresolved "cannot create symbolic link"
printf 'c\n' >mid || pc_unresolved "cannot create mid"
touch -d 2010-01-02T15:04:05Z mid || pc_unresolved "cannot set the time of mid"
printf 'c\n' >late || pc_unresolved "cannot create late"
touch -d 2037-01-02T15:04:05Z late || pc_unresolved "cannot set the time of late"
pc_test
got=$(find -H mid -newer link 2>err </dev/null) || pc_fail "find -H -newer link: nonzero status: $(cat err)"
[ "$got" = mid ] || pc_fail "-H: the time of the referenced file was not used: '$got'"
got=$(find -L mid -newer link 2>err </dev/null) || pc_fail "find -L -newer link: nonzero status: $(cat err)"
[ "$got" = mid ] || pc_fail "-L: the time of the referenced file was not used: '$got'"
got=$(find mid -newer dang 2>err </dev/null) || pc_fail "find -newer dangling: nonzero status: $(cat err)"
[ -z "$got" ] || pc_fail "mid is newer than a dangling link created now: '$got'"
got=$(find late -newer dang 2>err </dev/null) || pc_fail "find late -newer dangling: nonzero status: $(cat err)"
[ "$got" = late ] || pc_fail "late is not newer than the dangling link itself: '$got'"
pc_pass
