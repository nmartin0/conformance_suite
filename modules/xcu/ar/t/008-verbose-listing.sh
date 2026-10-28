# ar -tv listing line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.008
# "ar -tv a f1" in the POSIX locale and with TZ=UTC prints one line with the
# nine-character mode, owner id/group id, the size in bytes, the abbreviated
# month, day, hour:minute and year of the file's modification time when it
# was archived, and the name.
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.008
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf '12345' >f1
chmod 640 f1 || pc_unresolved "setup: chmod failed"
TZ=UTC touch -t 200203041507 f1 || pc_unresolved "setup: touch failed"
ar -rc a f1 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: cannot build the archive"
set -- $(ls -ln f1)
ids="$3/$4"
pc_test
line=$(TZ=UTC LC_ALL=C ar -tv a f1 </dev/null)
set -- $line
[ "$#" -eq 8 ] || pc_fail "listing has $# fields: $line"
[ "${1%"${1#?????????}"}" = 'rw-r-----' ] || pc_fail "mode field is '$1'"
[ "$2" = "$ids" ] || pc_fail "owner field is '$2', expected $ids"
[ "$3" = 5 ] || pc_fail "size field is '$3'"
[ "$4" = Mar ] || pc_fail "month field is '$4'"
d=${5#0}
[ "$d" = 4 ] || pc_fail "day field is '$5'"
h=${6%:*}; h=${h#0}
m=${6#*:}; m=${m#0}
{ [ "$h" = 15 ] && [ "$m" = 7 ]; } || pc_fail "time field is '$6'"
[ "$7" = 2002 ] || pc_fail "year field is '$7'"
[ "$8" = f1 ] || pc_fail "name field is '$8'"
pc_pass
