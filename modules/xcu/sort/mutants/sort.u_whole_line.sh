# sort -u compares whole lines instead of keys
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref u_whole_line "$@"; }
