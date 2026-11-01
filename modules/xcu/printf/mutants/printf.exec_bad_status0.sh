# the printf found on PATH exits zero for a bad number
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/printfref.sh"
pc_pfref_exec bad_status0
