# c17 -I header search order
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.c17.005
# Angle-bracket headers are found in the -I directories in the order given;
# a quoted header is found in the directory of the including file before any
# -I directory.
. "$PC_CORE/pc.sh"
pc_begin XCU.c17.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mkdir inc1 inc2 || pc_unresolved "setup: mkdir failed"
printf '#define V 1\n' >inc1/h.h
printf '#define V 2\n' >inc2/h.h
printf '#define V 3\n' >h.h
printf '#include <h.h>\nint main(void) { return V; }\n' >t.c
printf '#include "h.h"\nint main(void) { return V; }\n' >q.c
pc_test
c17 -I inc1 -I inc2 -o p1 t.c >out 2>err </dev/null || pc_fail "-I inc1 -I inc2: nonzero status"
./p1 </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "-I inc1 -I inc2 found header $rc, expected inc1"
c17 -I inc2 -I inc1 -o p2 t.c >out 2>err </dev/null || pc_fail "-I inc2 -I inc1: nonzero status"
./p2 </dev/null
rc=$?
[ "$rc" -eq 2 ] || pc_fail "-I inc2 -I inc1 found header $rc, expected inc2"
c17 -I inc1 -o p3 q.c >out 2>err </dev/null || pc_fail "quoted include: nonzero status"
./p3 </dev/null
rc=$?
[ "$rc" -eq 3 ] || pc_fail "quoted include found header $rc, expected the one next to the source"
pc_pass
