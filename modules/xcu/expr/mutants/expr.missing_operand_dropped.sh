# expr accepts an operator with no right operand and gives the left one
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/exprwrap.sh"
pc_exprmut missing_operand_dropped
