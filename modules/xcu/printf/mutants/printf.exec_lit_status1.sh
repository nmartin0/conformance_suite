# the printf found on PATH exits 1 although everything succeeded
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/printfref.sh"
pc_pfref_exec lit_status1
