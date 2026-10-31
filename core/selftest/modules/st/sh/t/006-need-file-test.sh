# Harness self-test. Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "$PC_CORE/pc.sh"
pc_begin ST.sh.006
pc_test
pc_need_file nosuch "the output"
pc_pass
