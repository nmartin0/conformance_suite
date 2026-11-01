# a conforming diff whose error status is 3 rather than 2
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/diffref.sh"
diff() { pc_diffref status3 "$@"; }
