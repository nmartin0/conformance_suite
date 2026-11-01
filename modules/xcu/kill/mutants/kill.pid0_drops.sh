# kill fault pid0_drops
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/killref.sh"
pc_killmut pid0_drops "${PC_MUTANT_FILE%/*}"
