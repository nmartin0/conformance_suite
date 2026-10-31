# sort ignores the type letters of a -k key
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref mod_ignored "$@"; }
