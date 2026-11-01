# -N with a leading zero is read as decimal
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/odref.sh"
od() { pc_odref N_oct_dec "$@"; }
