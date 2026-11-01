# when output would grow, the input file is changed
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cmut.sh"
compress() { pc_cm incr_modifies_input "$@"; }
