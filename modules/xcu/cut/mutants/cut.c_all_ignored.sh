# cut -c ignores the list and writes the whole line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cutref.sh"
cut() { pc_cutref all_ignored c "$@"; }
