# expr exits 1 for a remainder by zero
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/exprwrap.sh"
pc_exprmut mod0_exit1
