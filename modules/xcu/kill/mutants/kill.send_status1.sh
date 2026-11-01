# kill fault send_status1
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/killref.sh"
pc_killmut send_status1 "${PC_MUTANT_FILE%/*}"
