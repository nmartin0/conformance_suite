# kill fault neg_as_positive
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/killref.sh"
pc_killmut neg_as_positive "${PC_MUTANT_FILE%/*}"
