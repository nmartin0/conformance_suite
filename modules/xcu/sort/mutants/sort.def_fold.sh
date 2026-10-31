# sort with no options compares lowercase letters as uppercase
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref def_fold "$@"; }
