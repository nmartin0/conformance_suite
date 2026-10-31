# the type letters of the last -k key apply to every key
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref mod_global_last "$@"; }
