# expr gives 0 instead of the null string when the group matched nothing
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/exprwrap.sh"
pc_exprmut match_group_fail_zero
