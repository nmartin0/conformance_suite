# kill fault neg_rejected
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/killref.sh"
pc_killmut neg_rejected "${PC_MUTANT_FILE%/*}"
