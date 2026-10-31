# sort -k ignores the character position of the key start
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref char_start_ignored "$@"; }
