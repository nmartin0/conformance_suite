# a conforming nm: symbols of equal value come out in reverse name order with -v (order among equal values is open)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/nmref.sh"
nm() { pc_nmref v_ties_reversed "$@"; }
