# expr gives the null string instead of 0 when both sides of | are false
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/exprwrap.sh"
pc_exprmut or_null_result
