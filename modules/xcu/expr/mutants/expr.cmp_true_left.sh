# expr gives the left operand instead of 1 for a true comparison
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/exprwrap.sh"
pc_exprmut cmp_true_left
