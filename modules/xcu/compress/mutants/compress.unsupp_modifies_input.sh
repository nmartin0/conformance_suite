# an unsupported algorithm changes the input file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cmut.sh"
compress() { pc_cm unsupp_modifies_input "$@"; }
