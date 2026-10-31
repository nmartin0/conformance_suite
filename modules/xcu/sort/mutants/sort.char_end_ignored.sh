# sort -k ignores the character position of the key end
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref char_end_ignored "$@"; }
