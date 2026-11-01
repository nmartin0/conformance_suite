# a conforming diff whose -u times carry no fraction
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because the fraction field may be empty (Diff -u Output Format).
. "${PC_MUTANT_FILE%/*}/../mutants/diffref.sh"
diff() { pc_diffref u_frac_none "$@"; }
