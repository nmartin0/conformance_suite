# Being a special built-in, a variable assignment before the null utility
# stays in effect after it, and its redirections are performed (a file named
# in an output redirection is created or truncated).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.colon.005
. "$PC_CORE/pc.sh"
pc_begin XCU.colon.005
printf old >pc.z
unset pcx
pc_test
pcx=y : >pc.z
[ "$pcx" = y ] || pc_fail "assignment before colon did not persist (pcx='$pcx')"
[ -f pc.z ] || pc_fail "redirection file missing"
[ -s pc.z ] && pc_fail "redirection did not truncate the file"
pc_pass
