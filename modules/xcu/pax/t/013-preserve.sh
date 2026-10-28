# pax -p and times
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.013
# Under umask 077, "-p p" restores mode 640; by default the modification
# time stored in the archive is restored, and with -p m it is not.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.013
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'x\n' >f1
chmod 640 f1 || pc_unresolved "setup: chmod failed"
touch -t 200101010000 f1 || pc_unresolved "setup: touch failed"
touch -t 200201010000 ref || pc_unresolved "setup: touch failed"
pax -w f1 >a 2>/dev/null </dev/null || pc_unresolved "setup: pax -w failed"
rm -f f1
pc_test
umask 077
pax -r -p p <a >o 2>err
rc=$?
umask 022
[ "$rc" -eq 0 ] || pc_fail "pax -r -p p: exit status $rc"
perms=$(ls -l f1 | cut -c1-10)
[ "$perms" = '-rw-r-----' ] || pc_fail "-p p restored mode $perms, expected -rw-r-----"
rm -f f1
pax -r <a >o 2>err || pc_fail "pax -r: nonzero status"
old=$(find f1 ! -newer ref -print)
[ -n "$old" ] || pc_fail "the archived modification time was not restored by default"
rm -f f1
pax -r -p m <a >o 2>err || pc_fail "pax -r -p m: nonzero status"
new=$(find f1 -newer ref -print)
[ -n "$new" ] || pc_fail "-p m still restored the modification time"
pc_pass
