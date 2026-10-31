# without operands cut fails instead of reading standard input
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cutref.sh"
cut() { pc_cutref no_operand_error any "$@"; }
