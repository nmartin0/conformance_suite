# kill fault send_stdout
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/killref.sh"
pc_killmut send_stdout "${PC_MUTANT_FILE%/*}"
