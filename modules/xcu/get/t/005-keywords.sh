# identification keywords and -k
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.get.005
# Keywords %M% %I% %Z% %W% %F% are replaced in the retrieved text and -k
# leaves them alone.
. "$PC_CORE/pc.sh"
pc_begin XCU.get.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'id %%M%% %%I%% %%Z%%\nw %%W%%\nF %%F%%\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
rm -f f
pc_test
get -s -p  s.f </dev/null >out 2>err || pc_fail "get : nonzero status"
printf '%b' 'id f 1.1 @(#)\nw @(#)f\t1.1\nF s.f\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "keywords were not replaced as expected: $(od -c out | head -3)"
get -s -p -k s.f </dev/null >out 2>err || pc_fail "get -k: nonzero status"
printf '%b' 'id %M% %I% %Z%\nw %W%\nF %F%\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "-k did not leave the keywords: $(od -c out | head -3)"
pc_pass
