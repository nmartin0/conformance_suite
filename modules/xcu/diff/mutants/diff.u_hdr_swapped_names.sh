# -u: the names in the two header lines are exchanged
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref u_hdr_swapped_names "$@"; }
