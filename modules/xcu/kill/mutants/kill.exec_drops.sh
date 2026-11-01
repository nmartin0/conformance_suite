# kill fault exec_drops
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/killref.sh"
pc_killmut exec_drops "${PC_MUTANT_FILE%/*}"
