# identification strings and terminators
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.what.001
# what writes the pathname, then each string after @(#) up to a double
# quote, >, newline, backslash, NUL or end of file, each preceded by a tab.
. "$PC_CORE/pc.sh"
pc_begin XCU.what.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'x@(#)one"y @(#)two>z @(#)three\n@(#)four\\q @(#)five\000junk @(#)tail' >f || pc_unresolved "setup: cannot write f"
pc_test
what f </dev/null >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "what: exit status $rc"
printf 'f:\n\tone\n\ttwo\n\tthree\n\tfour\n\tfive\n\ttail\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "what wrote: $(tr '\n' '|' <out)"
pc_pass
