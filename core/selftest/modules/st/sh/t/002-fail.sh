# Harness self-test. Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "$PC_CORE/pc.sh"
pc_begin ST.sh.002
pc_test
pc_fail "planted failure"
