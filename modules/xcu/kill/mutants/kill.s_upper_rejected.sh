# kill fault s_upper_rejected
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/killref.sh"
pc_killmut s_upper_rejected "${PC_MUTANT_FILE%/*}"
