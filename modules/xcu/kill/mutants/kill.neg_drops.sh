# kill fault neg_drops
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/killref.sh"
pc_killmut neg_drops "${PC_MUTANT_FILE%/*}"
