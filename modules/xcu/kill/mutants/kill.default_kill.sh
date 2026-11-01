# kill fault default_kill
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/killref.sh"
pc_killmut default_kill "${PC_MUTANT_FILE%/*}"
