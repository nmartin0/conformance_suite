# kill fault s_usr1_as_usr2
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/killref.sh"
pc_killmut s_usr1_as_usr2 "${PC_MUTANT_FILE%/*}"
