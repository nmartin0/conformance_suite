# expr ignores the minus sign of an integer operand
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/exprwrap.sh"
pc_exprmut neg_dropped
