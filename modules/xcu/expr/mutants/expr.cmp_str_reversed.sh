# expr compares strings in reverse byte order
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/exprwrap.sh"
pc_exprmut cmp_str_reversed
