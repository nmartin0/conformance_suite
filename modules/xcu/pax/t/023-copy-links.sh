# pax -rw -l: hard links between the source and the destination hierarchy
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.023
# "pax -rw -l src dest" makes dest/src/f a hard link to src/f (same serial
# number); with -L a symbolic link met in the hierarchy is linked to the
# file it refers to.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.023
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mkdir src dest dest2 || pc_unresolved "setup: mkdir failed"
printf 'x\n' >src/f || pc_unresolved "setup: cannot write src/f"
ln -s f src/s || pc_unresolved "setup: ln -s failed"
ino() { ls -i "$1" | awk '{ print $1 }'; }
pc_test
pax -rw -l src dest >o 2>err </dev/null || pc_fail "pax -rw -l: nonzero status"
[ "$(cat dest/src/f 2>/dev/null)" = x ] || pc_fail "dest/src/f was not copied"
a=$(ino src/f); b=$(ino dest/src/f)
[ -n "$a" ] && [ "$a" = "$b" ] || pc_fail "dest/src/f (serial number '$b') is not a hard link to src/f ('$a')"
pax -rw -l -L src dest2 >o 2>err </dev/null || pc_fail "pax -rw -l -L: nonzero status"
c=$(ino dest2/src/s)
[ -n "$c" ] && [ "$c" = "$a" ] || pc_fail "with -l -L, dest2/src/s (serial number '$c') is not a hard link to the file src/s refers to ('$a')"
pc_pass
