# a conforming diff whose -u times carry a one-digit fraction
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/diffref.sh"
diff() { pc_diffref u_frac_one "$@"; }
