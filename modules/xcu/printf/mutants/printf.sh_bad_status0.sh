# the shell's printf exits zero for a bad number
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/printfref.sh"
pc_pfref_sh bad_status0
