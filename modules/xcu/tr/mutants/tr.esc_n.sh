# the escape backslash-n stands for the letter n
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref esc_n "$@"; }
