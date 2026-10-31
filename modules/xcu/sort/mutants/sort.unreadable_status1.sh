# sort exits 1 for a file that cannot be read
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref unreadable_status1 "$@"; }
