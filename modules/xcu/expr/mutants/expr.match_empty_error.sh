# expr reports an error when the string to match is empty
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/exprwrap.sh"
pc_exprmut match_empty_error
