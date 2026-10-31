# Harness self-test. Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "$PC_CORE/pc.sh"
pc_begin ST.sh.008
: >f || pc_fail "cannot make f"
pc_test
pc_need_file f
mkdir d || pc_fail "mkdir"
pc_need_file d
pc_pass
