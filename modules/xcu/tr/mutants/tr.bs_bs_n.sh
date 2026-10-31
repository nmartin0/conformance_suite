# a doubled backslash followed by n stands for a backslash and a newline
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref bs_bs_n "$@"; }
