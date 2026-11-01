# kill fault only_last
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/killref.sh"
pc_killmut only_last "${PC_MUTANT_FILE%/*}"
