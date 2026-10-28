# mkstemp
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.011
# mkstemp gives the pathname of a file it created from the template
# argument.
. "$PC_CORE/pc.sh"
pc_begin XCU.m4.011
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >in5 <<'EOM'
mkstemp(`pcmXXXXXX')
EOM
pc_test
m4 in5 >out 2>err </dev/null || pc_fail "mkstemp: nonzero status"
name=$(cat out)
case $name in *pcm??????*) ;; *) pc_fail "mkstemp gave '$name'" ;; esac
[ -f "$name" ] || pc_fail "mkstemp did not create '$name'"
rm -f "$name"
pc_pass
