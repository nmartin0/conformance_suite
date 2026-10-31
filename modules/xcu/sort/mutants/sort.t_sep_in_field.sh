# sort -t includes the separator in the field that follows it
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref t_sep_in_field "$@"; }
