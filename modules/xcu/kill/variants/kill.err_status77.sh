# a conforming kill: every failure is reported by exit status 77 (any non-zero value is allowed)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/killref.sh"
pc_killmut err_status77 "${PC_MUTANT_FILE%/*}/../mutants"
