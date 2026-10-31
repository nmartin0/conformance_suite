# sort exits 1 when it processed other files but one could not be read
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref unreadable_partial "$@"; }
