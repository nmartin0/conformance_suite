# a conforming diff whose -u times carry a one-digit fraction
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because the fraction may be a point and at least one digit; the file times here are whole seconds.
. "${PC_MUTANT_FILE%/*}/../mutants/diffref.sh"
diff() { pc_diffref u_frac_one "$@"; }
