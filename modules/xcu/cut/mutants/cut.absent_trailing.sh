# an absent field leaves a stray delimiter
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cutref.sh"
cut() { pc_cutref absent_trailing f "$@"; }
