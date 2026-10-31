# sort -i ignores printable punctuation as well
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref i_drops_punct "$@"; }
