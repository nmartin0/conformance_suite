# test: -nt and -ot are exchanged (in the standalone test only)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/testref.sh"
pcT_plant nt_ot_swap t
