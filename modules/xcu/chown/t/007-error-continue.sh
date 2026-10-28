# If one operand cannot be changed, chown diagnoses it, handles the others, and exits non-zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chown.007
. "$PC_CORE/pc.sh"
pc_begin XCU.chown.007
: >f || pc_fail "cannot create f"
uid=$(id -u) || pc_fail "cannot determine the user ID"
pc_test
chown "$uid" nonexistent f >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "chown exited 0 although a file does not exist"
[ -s err ] || pc_fail "no diagnostic"
pc_pass
