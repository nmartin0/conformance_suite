# pax -x pax: a time that is not a whole second goes in an mtime record
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.045
# For a file whose modification time has a half second, "pax -w -x pax"
# writes an mtime record whose value is the time in seconds since the Epoch
# with the digit 5 for the tenths (1041519845.5).
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.045
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'x\n' >b && printf 'x\n' >a || pc_unresolved "setup: cannot write files"
touch -d 2003-01-02T15:04:05Z b && touch -d 2003-01-02T15:04:05.5Z a || pc_unresolved "setup: touch -d with a fraction failed"
[ -n "$(find a -newer b -print)" ] || pc_notinuse "the file system keeps no sub-second modification times"
fld() { dd if="$1" bs=1 skip="$2" count="$3" 2>/dev/null | tr -d '\000'; }
num() { echo $((0$(fld "$1" "$2" "$3"))); }
pc_test
pax -w -x pax a >arch 2>err </dev/null || pc_fail "pax -w -x pax: nonzero status"
t=$(fld arch 156 1)
[ "$t" = x ] || pc_fail "the first block has typeflag '$t': no extended header was written for a time that ustar cannot hold"
s=$(num arch 124 12)
dd if=arch bs=1 skip=512 count="$s" 2>/dev/null | grep -E '^[0-9]+ mtime=1041519845\.50*$' >/dev/null || pc_fail "no mtime record holding 1041519845.5: $(dd if=arch bs=1 skip=512 count="$s" 2>/dev/null)"
pc_pass
