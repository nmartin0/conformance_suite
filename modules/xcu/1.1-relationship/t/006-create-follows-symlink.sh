# When a utility creates a regular file and the name is an existing symbolic link, the link is followed: the target is written, and a missing target is created; the link itself stays a link
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.1-relationship.006
. "$PC_CORE/pc.sh"
pc_begin XCU.1.1-relationship.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
ln -s target link 2>/dev/null && [ -h link ] || pc_notinuse "symbolic links cannot be created here"
printf 'old\n' >old || pc_unresolved "write"
ln -s old link2 || pc_unresolved "ln -s"
pc_test
printf 'data\n' | tee link >/dev/null || pc_fail "tee through a dangling link failed"
[ -h link ] || pc_fail "the symbolic link was replaced"
[ -f target ] && [ "$(cat target)" = data ] || pc_fail "the missing target was not created"
printf 'more\n' | tee link >/dev/null || pc_fail "tee through a link failed"
[ -h link ] && [ "$(cat target)" = more ] || pc_fail "the target was not written through the link"
: >link || pc_fail "a redirection through a link failed"
[ -h link ] && [ ! -s target ] || pc_fail "the redirection did not empty the target"
sort -o link2 /dev/null || pc_fail "sort -o through a link failed"
[ -h link2 ] && [ ! -s old ] || pc_fail "sort -o did not write the target"
pc_pass
