# sort -c -u does not report a duplicate key
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref u_dup_ok "$@"; }
