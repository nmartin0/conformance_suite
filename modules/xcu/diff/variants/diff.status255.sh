# a conforming diff whose error status is 255
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/diffref.sh"
diff() { pc_diffref status255 "$@"; }
