# sort does not complete an incomplete last line, so it joins the next file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref glue_files "$@"; }
