# expr writes no diagnostic for an error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/exprwrap.sh"
pc_exprmut invalid_no_diag
