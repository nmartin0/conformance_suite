# expr gives the null string instead of 0 for a false comparison
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/exprwrap.sh"
pc_exprmut cmp_false_empty
