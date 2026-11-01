# ar -tv listing line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.008
# "ar -tv a f1" in the POSIX locale and with TZ=UTC prints one line
# with the nine-character mode, owner id/group id, the size in
# bytes, the abbreviated month, day, hour:minute and year of the
# file's modification time when it was archived (shown in the zone TZ
# selects), and the name; without operands every member is listed
# in archive order.
# MUTANT    ar.tv_size_plus1
# MUTANT    ar.tv_size_kilo
# MUTANT    ar.tv_mode_wrong
# MUTANT    ar.tv_ids_swapped
# MUTANT    ar.tv_uid_twice
# MUTANT    ar.tv_month_lower
# MUTANT    ar.tv_month_full
# MUTANT    ar.tv_month_next
# MUTANT    ar.tv_day_next
# MUTANT    ar.tv_hour_next
# MUTANT    ar.tv_min_next
# MUTANT    ar.tv_hour_min_swapped
# MUTANT    ar.tv_year_next
# MUTANT    ar.tv_12hour
# MUTANT    ar.tv_year_before_time
# MUTANT    ar.tv_no_ids
# MUTANT    ar.tv_no_name
# MUTANT    ar.tv_name_first
# MUTANT    ar.tv_time_now
# MUTANT    ar.tv_ignores_tz
# MUTANT    ar.tv_status1
# MUTANT    ar.tv_operand_ignored
# MUTANT    ar.tv_reverse
# MUTANT    ar.tv_stderr
# VARIANT   ar.tv_unpadded
# VARIANT   ar.tv_mode_flag
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.008
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf '12345' >f1
chmod 640 f1 || pc_unresolved "setup: chmod failed"
# Give the file distinct owner and group ids where the privilege allows it.
chown 4242:4343 f1 2>/dev/null
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
# The time is shown in the zone TZ selects: a zone five hours behind.
line=$(TZ=EST5 LC_ALL=C ar -tv a f1 </dev/null)
set -- $line
[ "$#" -eq 8 ] || pc_fail "listing in another zone has $# fields: $line"
d=${5#0}
h=${6%:*}; h=${h#0}
m=${6#*:}; m=${m#0}
{ [ "$4" = Mar ] && [ "$d" = 4 ] && [ "$h" = 10 ] && [ "$m" = 7 ] && [ "$7" = 2002 ]; } || pc_fail "with TZ=EST5 the time is shown as '$4 $5 $6 $7', expected Mar 4 10:07 2002"
# A second file, in the next day and a different zone, and an empty one.
: >f2
chmod 755 f2 || pc_unresolved "setup: chmod failed"
TZ=UTC touch -t 199912312359 f2 || pc_unresolved "setup: touch failed"
ar -rc b f2 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: cannot build the archive"
line=$(TZ=UTC LC_ALL=C ar -tv b f2 </dev/null)
set -- $line
[ "$#" -eq 8 ] || pc_fail "second listing has $# fields: $line"
[ "${1%"${1#?????????}"}" = 'rwxr-xr-x' ] || pc_fail "second mode field is '$1'"
[ "$3" = 0 ] || pc_fail "second size field is '$3'"
h=${6%:*}; h=${h#0}
m=${6#*:}; m=${m#0}
d=${5#0}
{ [ "$4" = Dec ] && [ "$d" = 31 ] && [ "$h" = 23 ] && [ "$m" = 59 ] && [ "$7" = 1999 ] && [ "$8" = f2 ]; } || pc_fail "second listing is '$line'"
# Without operands every member is listed, in archive order, one per line.
ar -rc b f1 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: cannot extend the archive"
TZ=UTC ar -tv b >out 2>err </dev/null || pc_fail "ar -tv b: nonzero status"
[ -f out ] || pc_fail "no output file"
[ "$(wc -l <out)" -eq 2 ] || pc_fail "ar -tv b did not write one line for each of two members"
set -- $(sed -n 1p out); [ "$8" = f2 ] || pc_fail "first line of ar -tv b names '$8', expected f2"
set -- $(sed -n 2p out); [ "$8" = f1 ] || pc_fail "second line of ar -tv b names '$8', expected f1"
[ "$3" = 5 ] || pc_fail "size on the second line is '$3'"
# With an operand only that member is listed.
TZ=UTC ar -tv b f1 >out 2>err </dev/null || pc_fail "ar -tv b f1: nonzero status"
[ -f out ] || pc_fail "no output file"
[ "$(wc -l <out)" -eq 1 ] || pc_fail "ar -tv b f1 did not write exactly one line"
set -- $(cat out); [ "$8" = f1 ] || pc_fail "ar -tv b f1 names '$8'"
pc_pass
