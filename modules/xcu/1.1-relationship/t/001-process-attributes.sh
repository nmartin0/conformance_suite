# A process created by a utility's fork-and-exec has the attributes of the creating process that the standard lists: current directory, file mode creation mask, open file descriptors, real and effective user and group IDs, supplementary groups and process group
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.1-relationship.001
. "$PC_CORE/pc.sh"
pc_begin XCU.1.1-relationship.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'fd3\n' >in3 || pc_unresolved "cannot write a file"
cat >child.sh <<'EOS'
printf '%s\n' "$(pwd -P)" "$(umask)" "$(id -r -u)" "$(id -u)" "$(id -r -g)" "$(id -g)" "$(id -G)" "$(cat <&3)" "$(ps -o pgid= -p $$ | tr -d ' ')"
EOS
umask 027
pc_test
. ./child.sh 3<in3 >par.out 2>/dev/null
${PC_SH:-sh} ./child.sh 3<in3 >chi.out 2>/dev/null || pc_fail "the child shell failed"
cmp -s par.out chi.out || pc_fail "attributes differ: parent $(tr '\n' '|' <par.out) child $(tr '\n' '|' <chi.out)"
[ "$(sed -n 8p par.out)" = fd3 ] || pc_fail "the open descriptor was not readable in the parent"
pc_pass
